from django.shortcuts import render, redirect, get_object_or_404
from django.contrib.auth.forms import UserCreationForm, AuthenticationForm
from django.contrib.auth import authenticate, login, user_logged_in, logout
from django.contrib.auth.decorators import login_required
from . import forms, models
from .models import UserProfile

def login_view(request):
    if request.user.is_authenticated:
        return redirect('homepage')
    if request.method == 'POST':
        form = AuthenticationForm(request.POST, data=request.POST)
        if form.is_valid():
            user = form.get_user()
            if user is not None:
                login(request, user)
                next_url = request.GET.get('next') or 'homepage'
                return redirect(next_url)
    else:
        form = AuthenticationForm()
    return render(request, 'html/login.html', {'form': form})

def logout_view(request):
    logout(request)
    return redirect('login')

@login_required(login_url='login/')
def homepage(request):
    if request.user.is_superuser:
        return render(request, 'html/admin.html', {'username': request.user.username}) 
    else:
        # ensure profile exists
        profile = getattr(request.user, 'profile', None)
        if profile is None:
            profile = UserProfile.objects.create(user=request.user)
        return render(request, 'html/index.html', {
            'username': request.user.username,
            'sitting': profile.sitting,
            'standing': profile.standing,
        })

@login_required(login_url='login/')
def set_preferred_height(request):
    if request.method == 'POST':
        sittingheight = request.POST.get('sitting')
        standingheight = request.POST.get('standing')
        try:
            sittingvalue = float(sittingheight) if sittingheight not in (None, '') else None
            standingvalue = float(standingheight) if standingheight not in (None, '') else None
        except (ValueError, TypeError):
            # invalid input — can add messages.error here
            return redirect('homepage')

        profile = getattr(request.user, 'profile', None)
        if profile is None:
            profile = UserProfile.objects.create(user=request.user)
        profile.sittingheight = sittingvalue
        profile.standingheight = standingvalue
        profile.save()
        return redirect('homepage')

    return redirect('homepage')
