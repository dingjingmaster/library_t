#!/usr/bin/env python3.6
# -*-encoding=utf8-*-

from fake_useragent import UserAgent

if __name__ == '__main__':
	ua = UserAgent()
	print(ua.ie)
	print(ua.msie)
	print(ua.opera)
	print(ua.chrome)
	print(ua.google)
	print(ua.firefox)
	print(ua.safari)

	exit(0)
