from django.urls import path
from . import views

app_name = 'pico_api'

urlpatterns = [
    path('users/', views.get_users, name='get_users'),
    path('login/', views.login_user, name='login_user'),
    path('create/', views.create_user, name='create_user' ),
    path('logout/', views.logout_user, name='logout_user'),
    path('users/<int:pk>', views.displayProfile, name='display_user')
]
