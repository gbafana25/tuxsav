from django.shortcuts import render, redirect
from django.contrib.auth import authenticate, logout
from django.contrib.auth.models import User 
from django.contrib.auth.decorators import login_required 
from django.views.decorators.csrf import csrf_exempt
from django.http import HttpResponse, JsonResponse
from django.utils import timezone
from django.utils.text import normalize_newlines
import uuid
import json
import base64
from . import serv

from .forms import LoginForm
from .forms import SignupForm 
from .models import Document, ApiUser 

# Create your views here.

# Home page
def home(request):
	return render(request, 'cloud/home.html', {})

# Setup tutorial 
def setup_guide(request):
	return render(request, 'cloud/setup-guide.html', {})

# signout page, login handled by django builtin: accounts/
def signout(request):
	logout(request)
	return render(request, 'cloud/home.html', {})

# signup page
def signup(request):
	if request.method == 'POST':
		su_form = SignupForm(request.POST)

		# create accout
		if su_form.is_valid():
			usr = User.objects.create_user(su_form.cleaned_data['username'], su_form.cleaned_data['email'], su_form.cleaned_data['password'])
			usr.save()
			aobj = ApiUser.objects.create(name=su_form.cleaned_data['username'])
			# API keys are uuids
			aobj.key = uuid.uuid4()
	
			aobj.save()
			print(aobj.name, aobj.key)
			redirect('/')

	else:
		su_form = SignupForm()
	return render(request, 'cloud/signup.html', {'form':su_form})


# loads data on all documents (except text)

@login_required
def dashboard(request):
	docs = Document.objects.filter(owner=request.user)
	akobj = ApiUser.objects.get(name=request.user)
	time_list = []
	for d in docs:
		time_list.append(serv.get_elapsed(d.modified_at))

	group = zip(docs, time_list)
	return render(request, 'cloud/dashboard.html', {'docs':group, 'key':akobj.key})

# see contents of document 

@login_required
def document_viewer(request, name):
	doc = Document.objects.get(owner=request.user, title=name)
	return render(request, 'cloud/document-viewer.html', {"doc":doc})

# updates contents of document on server

@csrf_exempt
def update(request):
	if request.method != 'POST':
		return HttpResponse(serv.fail())

	jbody = json.loads(request.body.decode('utf-8'))
	# API key verified
	if(serv.check_api_key(jbody["key"], jbody["username"])):
		# get username from ApiUser object
		au = ApiUser.objects.get(name=jbody['username'])
		# use username string to get actual user object
		u = User.objects.get(username=au.name)
		# user object and title passed to Document getter
		doc = Document.objects.get(owner=u, title=jbody['doc_name'])
		doc.text = base64.b64decode(jbody["data"]).decode('ascii')
		doc.source_machine = jbody["host_name"].replace(" ", "")
		doc.current_source_file = jbody["local_name"]
		# make sure newlines appear in the HTML
		normalize_newlines(doc.text);
		
		# update modification time
		doc.modified_at = timezone.now()
		doc.save()

		return HttpResponse(serv.success(), content_type="application/json")
	
	else:
		return HttpResponse(serv.fail(), content_type="application/json")

# create a document object on the server

@csrf_exempt
def create(request):	
	if request.method != 'POST':
		return HttpResponse(serv.fail(), content_type="application/json")

	jbody = json.loads(request.body.decode('utf-8'))
	# check API key
	if(serv.check_api_key(jbody["key"], jbody["username"])):
		au = ApiUser.objects.get(name=jbody['username'])
		u = User.objects.get(username=au.name)
		doc = Document.objects.create(owner=u, title=jbody["doc_name"])
		doc.save()

		return HttpResponse(serv.success(), content_type="application/json")
	else:
		return HttpResponse(serv.fail(), content_type="application/json")

# delete document object from server

@csrf_exempt
def delete_document(request):
	if request.method != 'POST':
		return HttpResponse(serv.fail(), content_type="application/json")

	jbody = json.loads(request.body.decode('utf-8'))
	# check API key
	if(serv.check_api_key(jbody["key"], jbody["username"])):
		au = ApiUser.objects.get(name=jbody['username'])
		u = User.objects.get(username=au.name)
		doc = Document.objects.get(owner=u, title=jbody['doc_name'])
		doc.delete()

		return HttpResponse(serv.success(), content_type="application/json")
	else:	
		return HttpResponse(serv.fail(), content_type="application/json")

# send multiple documents to server

@csrf_exempt
def multi_update(request):
	if request.method != 'POST':
		return HttpResponse(serv.fail(), content_type="application/json")

	jbody = json.loads(request.body.decode('utf-8'))
	#print(jbody)
	if(serv.check_api_key(jbody["key"], jbody["username"])):
		# get username from ApiUser object
		au = ApiUser.objects.get(name=jbody['username'])
		# use username string to get actual user object
		u = User.objects.get(username=au.name)

		# user object and title passed to Document getter
		# loop through and update documents
			
		for c in range(0, len(jbody["doc_name"])):
			doc = Document.objects.get(owner=u, title=jbody["doc_name"][c])
			doc.text = base64.b64decode(jbody["data"][c]).decode('ascii')
			doc.source_machine = jbody["host_name"].replace(" ", "")
			doc.current_source_file = jbody["local_name"][c]
			# make sure newlines appear in the HTML
			normalize_newlines(doc.text);
		
			# update modification time
			doc.modified_at = timezone.now()
			doc.save()
		

	else:
		return HttpResponse(serv.fail(), content_type="application/json")
	

	return HttpResponse(serv.success(), content_type="application/json")


# send document data to client

@csrf_exempt
def fetch(request):
	if request.method != 'POST':
		return HttpResponse(serv.fail(), content_type="application/json")

	jbody = json.loads(request.body.decode('utf-8'))
	
	if(serv.check_api_key(jbody["key"])):
		# get document
		au = ApiUser.objects.get(name=jbody['username'])
		u = User.objects.get(username=au.name)
		doc = Document.objects.get(title=jbody["doc_name"], owner=u)

		resp_data = {"file_data": doc.text}	
		return HttpResponse(json.dumps(resp_data), content_type="application/json")
	else:
		return HttpResponse(serv.fail(), content_type="application/json")
