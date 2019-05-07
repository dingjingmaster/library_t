#!/usr/bin/env python3.6
# -*-encoding=utf8-*-
from setuptools import setup
from setuptools import find_packages

setup(
	name='spider',
	version='0.0.1',
	description='网页抓取相关工具/库',
	author='dingjing',
	packages=find_packages('spider'),           # 要打包的项目文件
	package_dir={'': 'spider'},
	include_package_date=True,                  # 自动打包文件夹内所有数据
	install_requires=[                          # 安装依赖的其它包
		'pyquery',
		'requests'
	]
)
