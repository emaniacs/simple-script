#!/usr/bin/env python
# -*- coding: utf8 -*-

# alexagrep.py
# Ardi nooneDOTnu1ATgmailDOTcom
# GNU GPL

import sys
import urllib2
from bs4 import BeautifulSoup
from re import sub

ATTR = [
    'Global ranks ',
    'Local ranks  ',
    'Reputation   ',
    'Rating       ',
]

def openLink(url):
    _url = 'http://www.alexa.com/siteinfo/'+ url
    opener = urllib2.build_opener()
    opener.addheaders = [('User-agent', 'Mozilla/5.0')]
    
    return opener.open(_url).read()

if __name__ == '__main__':
    try:
        url = sys.argv[1]
    except:
        print 'Error url'
        sys.exit()
        
    html = openLink(url)
    soup = BeautifulSoup(html)
    tds = soup.find('tr', {'class':'data-row1'}).findAll('td')
    i=0
    
    for td in tds:
        print ATTR[i] +' => '+ sub(r'\s+', ' ', td.find('div', {'class':'data'}).text.strip())
        i = i + 1
    
