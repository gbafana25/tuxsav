from .models import Document, ApiUser 
import json

def check_api_key(key):
	try:
		chk = ApiUser.objects.get(key=key)	
		return True
	except:
		return False


def success():
	return json.dumps({'status':'success'})

def fail():
	return json.dumps({'status':'failure'})
