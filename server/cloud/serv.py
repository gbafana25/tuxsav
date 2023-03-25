from .models import Document, ApiUser 
from django.utils import timezone
import json

def check_api_key(key, username):
	try:
		chk = ApiUser.objects.get(key=key)	
		if chk.name != username: 
			return False
		return True
	except:
		return False

# returns array with strings of elapsed time

def get_elapsed(last_modified):
	now = timezone.now() 
	tstr = ""
	days = now.day - last_modified.day
	hours = now.hour - last_modified.hour
	minutes = now.minute - last_modified.minute
	seconds = now.second - last_modified.second

	if days != 0:
		tstr += str(days) + " day(s) "
	elif hours != 0:
		tstr += str(hours) + " hour(s) "
	elif minutes != 0: 
		tstr += str(minutes) + " minutes(s) "
	elif seconds != 0:
		tstr += str(seconds) + " second(s) ago"

	return tstr 


def success():
	return json.dumps({'status':'success'})

def fail():
	return json.dumps({'status':'failure'})
