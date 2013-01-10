from django.conf.urls.defaults import patterns, include, url

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()

urlpatterns = patterns('',
    url(r'^cloudsto/$', 'cloudsto.views.index'),
    url(r'^cloudsto/register$', 'cloudsto.views.register'),
    url(r'^cloudsto/save$', 'cloudsto.views.save'),
    url(r'^cloudsto/receive$', 'cloudsto.views.receive'),
    # Examples:
    # url(r'^$', 'mesp.views.home', name='home'),
    # url(r'^mesp/', include('mesp.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    # url(r'^admin/', include(admin.site.urls)),
)
