# WoT map viewer
# 
# author: Juho Peltonen
# license: GPL3, see gpl-3.0.txt included in the package

XPM_MOD_VERSION = '1.0.0'
XPM_MOD_URL = 'https://github.com/juho-p/wot-map-viewer'
XPM_MOD_UPDATE_URL = ''
XPM_GAME_VERSIONS = ['0.9.2']


def log(text):
    import datetime
    ds = datetime.time.strftime(datetime.datetime.now().time(), '%H:%M')
    print 'map-view %s: %s' % (ds, text)

def run_server():
    import server
    log('run server...')
    try:
        while True:
            server.run('res\\packages', '127.0.0.1', 4444)
            log('stopped, restarting...')
    except:
        log('* Crashed *')
        import traceback
        traceback.print_exc()
    log('Server stopped!')

def init():
    log('starting..')

    try:
        import threading
        thread = threading.Thread(target=run_server, args=())
        thread.setDaemon(True)
        thread.start()

        log('thread started..')
    except:
        import traceback
        traceback.print_exc()

init()
