from django.db import models
from django.contrib.auth.models import User
from django.utils import timezone


class DeviceLoginSession(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE, related_name='device_sessions')
    login_time = models.DateTimeField(default=timezone.now)
    logout_time = models.DateTimeField(null=True, blank=True)

    def is_active(self):
        return self.logout_time is None

    # call this method to end the session when action logout is requested through the pico
    def end(self):
        self.logout_time = timezone.now()
        self.save()

    def __str__(self):
        return f"{self.user.username} session at {self.login_time}"


# action model bound to device login session through ForeignKey, to fetch all actions for a session use session.actions.all()
class UserAction(models.Model):
    ACTION_CHOICES = [
        ('sit', 'Sit'),
        ('stand', 'Stand'),
    ]

    session = models.ForeignKey(
        DeviceLoginSession, 
        on_delete=models.CASCADE, 
        related_name='actions'
    )

    # action can be 'sit' or 'stand', the mode selected by the user on the device
    action = models.CharField(max_length=10, choices=ACTION_CHOICES)
    # timestamp of the action. needed for logging when the action was taken
    timestamp = models.DateTimeField(default=timezone.now)

    def __str__(self):
        return f"{self.action} at {self.timestamp} ({self.session.user.username})"
