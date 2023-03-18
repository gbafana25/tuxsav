from django.urls import path, include
from . import views

urlpatterns = [
	path('', views.home, name='home'),
	path('dashboard', views.dashboard, name='dashboard'),
	path('accounts/', include('django.contrib.auth.urls')),
	path('signup', views.signup, name='signup'),
	path('update/', views.update, name='update'),
	path('create/', views.create, name='create'),
]
