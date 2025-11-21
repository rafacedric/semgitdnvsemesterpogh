from django.contrib import admin
from django.contrib.admin import AdminSite

class MyAdminSite(AdminSite):
    def has_permission(self, request):
        # Only allow superusers
        return request.user.is_active and request.user.is_superuser

admin_site = MyAdminSite(name='myadmin')