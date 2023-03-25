from django.db import models
import uuid
from django.utils import timezone
from django.conf import settings
from django.contrib.auth.models import User

# Create your models here.
class Document(models.Model):
	owner = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete=models.CASCADE)
	title = models.CharField(max_length=50)
	text = models.TextField()
	created_at = models.DateTimeField(default=timezone.now)
	modified_at = models.DateTimeField(default=timezone.now)

	# machine metadata
	current_source_file = models.TextField(null=True)
	source_machine = models.TextField(null=True)

	def __str__(self):
		return self.title

class ApiUser(models.Model):	
	name = models.CharField(primary_key=True, max_length=100)
	key = models.UUIDField(primary_key=False, default=uuid.uuid4, editable=True, null=True)

	

