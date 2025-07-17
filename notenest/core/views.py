from django.shortcuts import render, redirect, get_object_or_404
from django.contrib.auth.decorators import login_required

from django.contrib.auth.models import User
from django.contrib.auth import authenticate, login, logout
from django.contrib import messages
from .models import Profile, Note



@login_required
def edit_profile(request):
    if request.method == 'POST':
        user = request.user
        profile = user.profile

        full_name = request.POST.get('full_name')
        email = request.POST.get('email')
        password = request.POST.get('password')
        confirm_password = request.POST.get('confirm_password')

        user.email = email
        profile.full_name = full_name

        if password:
            if password == confirm_password:
                user.set_password(password)
                update_session_auth_hash(request, user)  # keeps user logged in
            else:
                messages.error(request, "Passwords do not match.")
                return redirect('edit_profile')

        user.save()
        profile.save()
        messages.success(request, "Profile updated successfully.")
        return redirect('home')

    return render(request, 'main/edit_profile.html')

@login_required
def edit_post(request, post_id):
    note = get_object_or_404(Note, id=post_id, user=request.user)
    if request.method == 'POST':
        note.note = request.POST.get('note')
        if request.FILES.get('attachment'):
            note.attachment = request.FILES['attachment']
        note.save()
        return redirect('home')
    return render(request, 'main/edit_post.html', {'post': note})

@login_required
def delete_post(request, post_id):
    note = get_object_or_404(Note, id=post_id, user=request.user)
    if request.method == 'POST':
        note.delete()
        return redirect('home')
    return redirect('home')



def home(request):
    if not request.user.is_authenticated:
        return redirect('login')

    if request.method == 'POST':
        note_text = request.POST.get('note')
        attachment = request.FILES.get('attachment')

        if note_text:
            Note.objects.create(user=request.user, note=note_text, attachment=attachment)
            messages.success(request, "Note posted successfully.")
            return redirect('home')

    posts = Note.objects.all().order_by('-created_at')
    return render(request, 'main/home.html', {'posts': posts})

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
