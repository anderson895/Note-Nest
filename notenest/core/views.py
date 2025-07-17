from django.shortcuts import render, redirect
from django.contrib.auth.models import User
from django.contrib.auth import authenticate, login, logout
from django.contrib import messages
from .models import Profile

def home(request):
    if not request.user.is_authenticated:
        return redirect('login')
    return render(request, 'main/home.html')

def register(request):
    if request.method == 'POST':
        full_name = request.POST['full_name']
        email = request.POST['email']
        username = request.POST['username']
        password = request.POST['password']
        avatar = request.FILES.get('avatar')

        if User.objects.filter(username=username).exists():
            messages.error(request, 'Username already exists')
            return redirect('register')

        user = User.objects.create_user(username=username, email=email, password=password)
        Profile.objects.create(user=user, full_name=full_name, avatar=avatar)

        messages.success(request, 'Account created. Please log in.')
        return redirect('login')
    return render(request, 'main/register.html')

def login_view(request):
    if request.method == 'POST':
        username = request.POST['username']
        password = request.POST['password']
        user = authenticate(request, username=username, password=password)
        if user:
            login(request, user)
            return redirect('home')
        else:
            messages.error(request, 'Invalid credentials')
            return redirect('login')
    return render(request, 'main/login.html')

def logout_view(request):
    logout(request)
    return redirect('login')
