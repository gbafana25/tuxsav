from django.shortcuts import render, redirect
from django.contrib.auth import authenticate, logout
from django.contrib.auth.models import User 
from django.contrib.auth.decorators import login_required 

from .forms import LoginForm
from .forms import SignupForm 
from .models import Document

# Create your views here.
def home(request):
	return render(request, 'cloud/home.html', {})

def signout(request):
	logout(request)
	return render(request, 'cloud/home.html', {})

"""
def login(request):
	if request.method == 'POST':
		if request.user.is_authenticated == False:
			login_form = LoginForm(request.POST)
			if login_form.is_valid():
				usr = authenticate(username=login_form.cleaned_data['username'], password=login_form.cleaned_data['password'])
				if usr is not None:
					#return render(request, 'cloud/dashboard.html', {})
					print(request.user)
					return redirect('/dashboard/')
		else:	
			if request.user.is_authenticated == True:
				#return render(request, 'cloud/dashboard.html', {})
				return redirect('/dashboard/')
	else:
		login_form = LoginForm()
	return render(request, 'cloud/login.html', {'form':login_form})
"""

def signup(request):
	if request.method == 'POST':
		su_form = SignupForm(request.POST)

		# create accout
		if su_form.is_valid():
			usr = User.objects.create_user(su_form.cleaned_data['username'], su_form.cleaned_data['email'], su_form.cleaned_data['password'])
			usr.save()
			redirect('')

	else:
		su_form = SignupForm()
	return render(request, 'cloud/signup.html', {'form':su_form})


@login_required
def dashboard(request):
	docs = Document.objects.filter(owner=request.user)
	#print(docs[0].text, docs[1].text)
	return render(request, 'cloud/dashboard.html', {'docs':docs})
