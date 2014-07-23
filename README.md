qt_google_task
==============

Couple of classes to work with google task API from Qt.
I will reformat it as separate library later and may be i will be able to build gui app on top of that.
## General Requirements
  You need libqjson (if i will get addidional free time i will port it to qt5, where json support embedded)

## How to use

```
#include <QSettings>  - i use this class for saving tokens and library parameters
```
  You have to set in your config file (which in my case ~/.config/MegawarpSoftware/goauth.conf) this values: client_id, client_secret and redirect_uri.
  I store secret data in config file to easily work with git(if you will get my client_secret from the repo history don't worry: it's outdated). 
  ```
#include "gtaskhelper.h" - helper class, which contains methods and data structures to work with Google task
#include "oauth2.h"  - class, that stores all the goods to work with OAuth2 and Google, gtaskhelper use this class
#include <QNetworkAccessManager> - We have to use onle the one instance of this class to make a get and post - requests, so, put the poinnet to your instance to the constructor of gtaskhelper
```
```
m_pOAuth2 = new OAuth2(this,qnam); // this will shows up typical Login Dialog for google auth.
connect(m_pOAuth2, SIGNAL(loginDone()), this, SLOT(slotLoginDone())); // in your slot you can try to get access token
```
If it isn't a first login you can use saved(so named refresh_token) to refresh access_token:
```
QString v = conf->value("refresh_token").toString();
    if(v.isEmpty())
        m_pOAuth2->startLogin(false);
    else
        slotLoginDone();
```
Inside slotLoginDone you do:
```
connect(m_pOAuth2, SIGNAL(AccessTokenArrived(QString)) , this, SLOT(slotTokenObtained(QString)) );
m_pOAuth2->getAccessToken();  // try to get access token, if ok then you can use gTaskHelper class to operate with api (I think api is intuitive)
```