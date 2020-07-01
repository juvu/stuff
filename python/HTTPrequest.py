
import urllib.request as asdf


for x in range(15,25):
    url = 'http://python.org/'
    headers={'Accept': str(x)}

    req = asdf.Request(url, None, headers)
    response = asdf.urlopen(req)
    the_page = response.read()
    print (the_page)




