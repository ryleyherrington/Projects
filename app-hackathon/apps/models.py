from django.db import models


class Idea(models.Model):
    name = models.CharField(max_length=255)
    author = models.CharField(max_length=255, null=True)  # null means anonymous
    description = models.TextField()
    post_time = models.DateTimeField(auto_now_add=True)


class Project(models.Model):
    name = models.CharField(max_length=255)
    description = models.TextField()


class Vote(models.Model):
    project = models.ForeignKey(Project)
    count = models.IntegerField(default=0)
    
    def increment(self):
        self.count += 1
