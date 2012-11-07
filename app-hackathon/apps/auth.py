'''
Created on Oct 9, 2012

@author: Dan
'''

import env

def current_user():
    if env.is_gae():
        from google.appengine.api import users
        return users.get_current_user()
    else:
        return None

def user_is_admin():
    if env.is_gae():
        from google.appengine.api import users
        return users.is_current_user_admin()
    else:
        return True  # dev user is always admin

def logged_in():
    return current_user() is not None

def gae_login_logout(request):
    from google.appengine.api import users
    
    if logged_in():
        url = users.create_logout_url(request.build_absolute_uri())
        text = 'Logout'
    else:
        url = users.create_login_url(request.build_absolute_uri())
        text = 'Login'
        
    return (text, url)

def django_login_logout(request):
    return ('Django log in not supported', '#')

def login_logout(request):
    if env.is_gae():
        return gae_login_logout(request)
    else:
        return django_login_logout(request)