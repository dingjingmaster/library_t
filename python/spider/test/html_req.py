#!/usr/bin/env python3.6
# -*-encoding=utf8-*-

from spider.get import Get

if __name__ == '__main__':
	g = Get('https://blog.csdn.net/yucicheung/article/details/79445350')
	print(g.html())
	exit(0)
