from django.db import models
from django.contrib.auth.models import User
from django.utils import timezone
from django.db.models.signals import post_save
from django.dispatch import receiver

class Table(models.Model):
    name = models.CharField(max_length=100)
    height = models.FloatField(help_text="Current height")


    def __str__(self):
        return f"{self.name} ({self.height} cm)"
    
class UserDesk(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE, related_name="table_user")
    table = models.ForeignKey(Table, on_delete=models.CASCADE, related_name="user_table")

class UserProfile(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE, related_name="profile")
    sitting = models.FloatField(null=True, blank=True, help_text="Preferred sitting height in cm")
    standing = models.FloatField(null=True, blank=True, help_text="Preferred standing height in cm")

    def __str__(self):
        return f"Profile: {self.user.username} (sitting height={self.sitting}) (standing height={self.standing})"

# create profile when a User is created
@receiver(post_save, sender=User)
def ensure_user_profile(sender, instance, created, **kwargs):
    if created:
        UserProfile.objects.create(user=instance)
    else:
        # if profile missing for some reason, create it
        if not hasattr(instance, 'profile'):
            UserProfile.objects.create(user=instance)

# example
# profile = request.user.profile
# profile.preferred_height = 72.5
# profile.save()
