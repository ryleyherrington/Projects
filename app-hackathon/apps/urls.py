from django.conf.urls.defaults import patterns, url

urlpatterns = patterns('apps.views',
    url(r'^$', 'index'),
    url(r'^index$', 'index'),
    url(r'^about$', 'about'),
    url(r'^faq$', 'faq'),
    url(r'^live$', 'live'),
    url(r'^ideas$', 'ideas'),
    
    url(r'^(?P<idea_id>\d+)/approve/$', 'approve'),
    url(r'^(?P<vote_id>\d+)/vote/$', 'vote'),
)
