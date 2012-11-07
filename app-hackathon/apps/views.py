from django.http import HttpResponseRedirect, Http404
from django.shortcuts import render_to_response, get_object_or_404
from django.template import RequestContext
from django.core.urlresolvers import reverse

from apps.models import Idea, Project, Vote

import auth
import env

def basic_response(template, request, extra_data={}):
    data = {'os': env.user_os(request),
            'browser': env.user_browser(request),
            'agent': env.user_agent(request)}
    
    if len(extra_data):
        data = dict(data.items() + extra_data.items())
    return render_to_response(template,
                              data,
                              context_instance=RequestContext(request))
    
def index(request):
    vote_list = Vote.objects.all().order_by('-count')
    return basic_response('apps/index.html',
                          request,
                          {'votes': vote_list})

def approve(request, idea_id):
    idea = get_object_or_404(Idea, pk=idea_id)
    
    proj = Project()
    proj.name = idea.name
    proj.description = idea.description
    proj.save()
    
    vote = Vote()
    vote.project = proj
    vote.save()
    
    idea.delete()
    
    return HttpResponseRedirect(reverse('apps.views.index'))

def vote(request, vote_id):
    v = get_object_or_404(Vote, pk=vote_id)
    v.increment()
    v.save()
    return HttpResponseRedirect(reverse('apps.views.index'))

def rules(request):
    return basic_response('apps/rules.html', request)

def about(request):
    return basic_response('apps/about.html', request)

def faq(request):
    return basic_response('apps/faq.html', request)

def live(request):
    return basic_response('apps/live.html', request)

def ideas(request):
    if request.method == 'GET':
        (login_text, login_url) = auth.login_logout(request)
        
        idea_list = Idea.objects.all().order_by('-post_time')
        return basic_response(
                'apps/ideas.html',
                request,
                {
                    'admin': auth.user_is_admin(),
                    'ideas': idea_list,
                    'login_url': login_url,
                    'login_text': login_text,
                })
    elif request.method == 'POST':
        idea = Idea()
        idea.name = request.POST['name']
        idea.description = request.POST['description']
        idea.author = auth.current_user()
        idea.save()
        return HttpResponseRedirect(reverse('apps.views.ideas'))
    else:
        raise Http404

def submission(request):
    return basic_response('apps/submission.html', request)
