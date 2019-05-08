#!/usr/bin/env python3.6
# -*-encoding=utf8-*-
import os
import time
import shutil
from spider.log import logging as log


def mkdir(path: str):
	os.mkdir(path, mode=0o777)


def copy_file(old: str, new: str):
	shutil.copy(old, new)
