# WoT map viewer
# 
# author: Juho Peltonen
# license: GPL3, see gpl-3.0.txt included in the package

XPM_MOD_VERSION = '1.1.0'
XPM_MOD_URL = 'https://github.com/juho-p/wot-map-viewer'
XPM_MOD_UPDATE_URL = ''
XPM_GAME_VERSIONS = ['0.9.2']


def log(text):
    import datetime
    ds = datetime.time.strftime(datetime.datetime.now().time(), '%H:%M')
    print 'map-view %s: %s' % (ds, text)

def read_config():
    import BigWorld
    from os import path
    cfg = { 'address': 'localhost', 'port': 4444, 'password': None }
    cfgfile = BigWorld.wg_resolveFileName(
            path.join(path.dirname(__file__), 'config.txt'))
    with open(cfgfile) as f:
        exec f.read() in cfg
    return cfg

def run_server():
    import server
    log('run server...')
    cfg = read_config()
    try:
        while True:
            server.run('res\\packages', cfg['address'], cfg['port'])
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
