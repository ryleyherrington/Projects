'''
Created on Oct 9, 2012

@author: Dan
'''
import re
import os

def is_gae():
    if (os.getenv('SERVER_SOFTWARE', '').startswith('Google App Engine') or
            'Development' in os.getenv('SERVER_SOFTWARE', '') or
            os.getenv('SETTINGS_MODE') == 'prod'):
        return True
    else:
        return False

def is_gae_dev():
    return 'Development' in os.getenv('SERVER_SOFTWARE', '')

def user_agent(request):
    return request.META['HTTP_USER_AGENT']

def user_os(request):
    '''Returns a best guess at the user's operating system.
    
    Parameters:
        request: the Django HTTP request object
    
    Returns:
        A string containing the best guess at the user's operating system, or
        'Unknown'
    
    Operating system information is parsed from the HTTP request's user agent,
    and may be spoofed.
    '''
    
    agent = user_agent(request)
    
    # start with the easy ones...
    if 'iPad' in agent:
        return 'iPad'
    elif 'iPhone' in agent:
        return 'iPhone'
    elif 'iPod' in agent:
        return 'iPod'
    elif 'Android' in agent:
        # Could potentially use these:
        # 
        # Android phone:  Android' + 'Chrome/[.0-9]* Mobile
        # Android tablet: Android' + 'Chrome/[.0-9]* (?!Mobile)
        if 'Mobile' in agent:
            return 'Android Mobile'
        else:
            return 'Android Tablet'
    elif 'Mac OS' in agent:
        return 'Mac OS'
    elif 'Linux' in agent:
        return 'Linux'
    elif 'Windows CE' in agent:
        return 'Windows CE'
    elif 'Windows Phone' in agent:
        return 'Windows Phone'
    elif 'Symbian' in agent:
        return 'Symbian'
    elif 'Windows' in agent:
        return 'Windows'
    elif 'BlackBerry' in agent:
        return 'BlackBerry'
    elif 'RIM Tablet' in agent:
        return 'RIM Tablet'
    else:
        # BSD and other Unix flavors fall in here. We can add support for them
        # later if necessary, but each flavor has it's own user agent, so I'd
        # rather not.
        return 'Unknown'

def user_browser(request):
    '''Returns a best guess at the user's browser.
    
    Parameters:
        request: the Django HTTP request object
    
    Returns:
        A string containing the best guess at the user's browser, or 'Unknown'
    
    Browser information is parsed from the HTTP request's user agent, and may be
    spoofed.
    '''
    agent = user_agent(request)
    
    if 'Chrome' in agent:
        return 'Chrome'
    elif 'Firefox' in agent:
        return 'Firefox'
    elif 'Opera' in agent:
        return 'Opera'
    elif 'MSIE' in agent:
        return 'MSIE'
    elif 'Netscape' in agent:
        return 'Netscape'
    elif 'BlackBerry' in agent:
        return 'BlackBerry'
    elif 'RIM Tablet' in agent:
        return 'RIM Tablet'
    elif 'Symbian' in agent:
        return 'Symbian'
    elif 'Safari' in agent:
        return 'Safari'
    else:
        return 'Unknown'