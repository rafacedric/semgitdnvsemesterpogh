from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.utils import timezone
import json
from django.contrib.auth.models import User
from .models import DeviceLoginSession
from django.contrib.auth import authenticate
# from django.contrib.auth.hashers import make_password


# GET /api/users/ for the pico to get the list of username that will be displayed to choose from
def get_users(request):
    
    # commented out to use user object instead
    # users = User.objects.values_list('username', flat=True)
    # return JsonResponse(list(users), safe=False)
    if request.method != "GET":
        return JsonResponse({'Error':'Method not allowed'}, status=405)
    
    user=User.objects.values('id', 'username')
    users = list(user)
    
    return JsonResponse(users, safe=False, status=200)
    

# POST /api/login/ when the user selects a username this request is made, then django will store the session as DeviceLoginSession
@csrf_exempt
def login_user(request):
    
    if request.method == "POST":
        data = json.loads(request.body)
        username = data.get("username")

        try:
            user = User.objects.get(username=username)
            
        except User.DoesNotExist:
            return JsonResponse({"error": "User not found"}, status=404)

        session = DeviceLoginSession.objects.create(
            user=user,
            login_time=timezone.now()
        )
        return JsonResponse({"message": "Login recorded", "session_id": session.id, "user_id":user.id, "username": username,
                             "admin":user.is_superuser})

    return JsonResponse({"error": "POST required"}, status=400)

# POST /api/logout/ to terminate the session
# to do: make the pico logout automatically if the user forgets to!
@csrf_exempt
def logout_user(request):
    if request.method == "POST":
        data = json.loads(request.body)
        username = data.get("username")

        try:
            user = User.objects.get(username=username)
        except User.DoesNotExist:
            return JsonResponse({"error": "User not found"}, status=404)

        # find the latest active session
        session = DeviceLoginSession.objects.filter(user=user, logout_time__isnull=True).last()
        if session:
            session.logout_time = timezone.now()
            session.save()
            return JsonResponse({"message": "Logout recorded"})

        return JsonResponse({"error": "No active session"}, status=400)

    return JsonResponse({"error": "POST required"}, status=400)

# POST create user for adding the user to the database
@csrf_exempt
def create_user(request):
    if request.method == 'POST':
        data = json.loads(request.body)
        username = data.get('username')
        password = data.get('password')
        firstName = data.get('firstName')
        lastName = data.get('lastName')
        email = data.get('email')
        
        if not username or not password:
            return JsonResponse({'error': 'Username or password required'})
        
        try:
            new_user = User.objects.create_user(username=username, password=password, first_name = firstName, last_name=lastName, email=email)
            print("Hashed password is:", password)
            return JsonResponse({'success':True, 'message':'User created successfully'})
        
        except json.JSONDecodeError:
            return JsonResponse({'error':'Invalid JSON'}, status=400)
            
    return JsonResponse({'error':'Method not allowed'}, status=405)

# POST view user profile
def displayProfile(request, pk):
    # data = json.loads(request.body)
    try:
        if request.method != "GET":
            return JsonResponse({'error':'Method not allowed'}, status=405)
        
        user= User.objects.get(pk=pk)
        
        data = {'id':pk, 'username':user.username}
        
        return JsonResponse(data, status=200)
    except User.DoesNotExist:
        return JsonResponse({'error': 'User not found'}, status=404)
    
# POST request for user to register sit/stand action during session. json expected: {"username": "user1", "action": "sit" or "stand"}
@csrf_exempt
def session_action(request):
    if request.method == "POST":
        data = json.loads(request.body)
        username = data.get("username")
        action = data.get("action")

        try:
            user = User.objects.get(username=username)
        except User.DoesNotExist:
            return JsonResponse({"error": "User not found"}, status=404)

        # find the latest active session
        session = DeviceLoginSession.objects.filter(user=user, logout_time__isnull=True).last()
        if session:
            # do the registring of the action here
            # for now just return success cause no model to store actions yet
            return JsonResponse({"message": f"Action '{action}' recorded for user '{username}'"})

        return JsonResponse({"error": "No active session"}, status=400)

    return JsonResponse({"error": "POST required"}, status=400)
        
        