#!/usr/bin/env python3.6
# -*-encoding=utf8-*-

import logging

logging.basicConfig(
	format='[%(asctime)s] %(filename)s line:%(lineno)05d [%(levelname)s]: %(message)s',
	level=logging.INFO,
	datefmt='%Y-%m-%d %H:%M:%S'
)
