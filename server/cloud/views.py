from django.shortcuts import render, redirect
from django.contrib.auth import authenticate, logout
from django.contrib.auth.models import User 
from django.contrib.auth.decorators import login_required 
from django.views.decorators.csrf import csrf_exempt
from django.http import HttpResponse, JsonResponse
import uuid
import json
from . import serv

from .forms import LoginForm
from .forms import SignupForm 
from .models import Document, ApiUser 

# Create your views here.
def home(request):
	return render(request, 'cloud/home.html', {})

def setup_guide(request):
	return render(request, 'cloud/setup-guide.html', {})

def signout(request):
	logout(request)
	return render(request, 'cloud/home.html', {})

def signup(request):
	if request.method == 'POST':
		su_form = SignupForm(request.POST)

		# create accout
		if su_form.is_valid():
			usr = User.objects.create_user(su_form.cleaned_data['username'], su_form.cleaned_data['email'], su_form.cleaned_data['password'])
			usr.save()
			aobj = ApiUser.objects.create(name=su_form.cleaned_data['username'])
			aobj.key = uuid.uuid4()
	
			aobj.save()
			print(aobj.name, aobj.key)
			redirect('/')

	else:
		su_form = SignupForm()
	return render(request, 'cloud/signup.html', {'form':su_form})


@login_required
def dashboard(request):
	docs = Document.objects.filter(owner=request.user)
	akobj = ApiUser.objects.get(name=request.user)
	#print(docs[0].text, docs[1].text)
	return render(request, 'cloud/dashboard.html', {'docs':docs, 'key':akobj.key})

@csrf_exempt
def update(request):
	if request.method != 'POST':
		return HttpResponse(serv.fail())

	jbody = json.loads(request.body.decode('utf-8'))
	if(serv.check_api_key(jbody["key"])):
		print("key verified")
		au = ApiUser.objects.get(name=jbody['username'])
		u = User.objects.get(username=au.name)
		doc = Document.objects.get(owner=u, title=jbody['doc_name'])
		doc.text = jbody["data"]
		doc.save()

		return HttpResponse(serv.success(), content_type="application/json")
	
	else:
		return HttpResponse(serv.fail(), content_type="application/json")


@csrf_exempt
def create(request):	
	jbody = json.loads(request.body.decode('utf-8'))
	print(jbody)
	if(serv.check_api_key(jbody["key"])):
		au = ApiUser.objects.get(name=jbody['username'])
		u = User.objects.get(username=au.name)
		doc = Document.objects.create(owner=u, title=jbody["doc_name"])
		doc.save()

		return HttpResponse(serv.success(), content_type="application/json")
	else:
		return HttpResponse(serv.fail(), content_type="application/json")

