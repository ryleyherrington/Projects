The Development Environment
===========================

For now, the best way to run the development environment is through the Django
dev server. To run this, first make sure all the Django 1.3 tools are installed
an in your `PATH`. Then, from the root of the project directory, run

    $ python manage.py syncdb

This will initialize the sqlite3 database as needed for you to run the site. To
start the server, run

    $ python manage.py runserver

Then navigate to [http://127.0.0.1:8000/](http://127.0.0.1:8000/) and begin
testing the site. If you make any changes to the models and need to update the
existing database, use

    $ python manage.py flush

__Be very careful__ not to run this command against the production database, as
it will clear the entire database. When I get the chance, I'll set up migrations
so we don't need this in the future.

Testing with Google App Engine
==============================

Once you have most of your changes implemented, test the site in Google App
Engine. You'll need to set up a local MySQL instance (or figure out why
`dev_appserver.py --use_sqlite` doesn't work) and configure it in `settings.py`.
You'll need to run `syncdb` as you did for Django. Once you've done this, start
the dev server using

    $ dev_appserver.py hackathon

Note: this needs to be run from the directory above the project.

Guidelines and Style
====================
* Make sure your dev environment (i.e. Django) works before you write any new
  code.
* Use Django utilities wherever possible. If you need any GAE utilities, make
  sure you implement it in such a way that the Django dev server doesn't blow up
  when you use it.
    * One way you might do this is to appropriately stub some functions. Take a
	  look at the code in `apps/auth.py` for an example of one way this could be
	  done.
    * Use `apps/env.py` to determine what environment you're operating in.
* Make sure your HTML is valid and well indented. It is really difficult to
  figure out what the tag hierarchy is when the HTML is malformed, poorly
  indented, and written by someone else.
* Try to keep your lines limited to 80 characters. I'm often working from a
  terminal, and lines reaching into the hundreds of characters in length are
  very unwieldy.
* Avoid using memcaches. Premature optimization only makes development more
  difficult. If we see some parts of the site that would benefit from their use,
  file a bug and I'll take care of it.
* Let me (Dan) know before you make a database change. Each time I've had to
  backport changes from the mainline into Django it has taken me several hours
  to hack, prune and mash the GAE interface into Django.
* Make sure your resources are in the right place. HTML templates go in
  `/templates/apps`. Stylesheets, images and scripts go in `/static` (or
  `/static/apps`, if you deem it appropriate).
* [Python style guide](http://www.python.org/dev/peps/pep-0008/)