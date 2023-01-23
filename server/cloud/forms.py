from django import forms

class LoginForm(forms.Form):
	username = forms.CharField()
	password = forms.CharField()
	
class SignupForm(forms.Form):
	username = forms.CharField()
	password = forms.CharField()
	email = forms.CharField()
