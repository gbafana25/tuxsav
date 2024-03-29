# Generated by Django 4.1.5 on 2023-03-15 04:39

from django.db import migrations, models
import uuid


class Migration(migrations.Migration):

    dependencies = [
        ('cloud', '0002_docuser'),
    ]

    operations = [
        migrations.CreateModel(
            name='ApiUser',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('name', models.TextField()),
                ('key', models.UUIDField(default=uuid.uuid4, null=True)),
            ],
        ),
        migrations.DeleteModel(
            name='DocUser',
        ),
    ]
