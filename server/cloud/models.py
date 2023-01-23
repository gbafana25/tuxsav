from django.db import models
from django.utils import timezone
from django.conf import settings

# Create your models here.
class Document(models.Model):
	owner = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete=models.CASCADE)
	title = models.CharField(max_length=50)
	text = models.TextField()
	created_at = models.DateTimeField(default=timezone.now)

	def __str__(self):
		return self.title
	
	
