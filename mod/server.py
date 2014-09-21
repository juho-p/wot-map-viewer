#!/usr/bin/python

# WoT map viewer
# 
# author: Juho Peltonen
# license: GPL3, see gpl-3.0.txt included in the package

import socket
import os

def vehicle_type(vehicle):
    from items.vehicles import VEHICLE_CLASS_TAGS
    result_set = vehicle['vehicleType'].type.tags & VEHICLE_CLASS_TAGS
    return next(iter(result_set))

def get_vehicle_basic_data(player, v):
    return {'name': v['name'],
            'tank': v['vehicleType'].type.userString,
            'max_health': v['vehicleType'].maxHealth,
            'type': vehicle_type(v),
            'alive': v['isAlive'],
            'team': 'f' if v['team'] == player.team else 'e'}

def relative_position(pos, bounds):
    pos = [pos[0], pos[2]]
    bot, top = bounds
    dims = (0, 1)
    diff = [top[n] - bot[n] for n in dims]
    return [float(pos[n] - bot[n]) / diff[n] for n in dims]

def get_vehicle_changes(v, map_bounds):
    data = {
        'pos': relative_position(v.position, map_bounds),
        'health': v.health,
    }
    return data

def is_in_game(player):
    return hasattr(player, 'arena') and player.vehicle is not None

class Info:
    def __init__(self):
        self._reset()

    def _reset(self):
        self.old_arena_id = None
        self.map_bounds = None
        self.data = None

    def _is_new_map(self, arena):
        aid = id(arena)
        if aid == self.old_arena_id:
            return False
        else:
            self.old_arena_id = aid
            return True
    def _update_vehicle_basic_info(self, player):
        game_vehicles = dict(player.arena.vehicles)
        vids = set(game_vehicles.keys())
        vehicles = self.data['vehicles']
        missing_ids = vids - set(vehicles.keys())
        for vid in missing_ids:
            v = game_vehicles[vid]
            if v.get('vehicleType', False):
                vehicles[vid] = get_vehicle_basic_data(player, v)

    def collect_data(self):
        import BigWorld
        player = BigWorld.player()
        if not is_in_game(player):
            self._reset()
            return self.data

        vehicles = dict(player.arena.vehicles)
        if self._is_new_map(player.arena):
            map_cfg = player.arena.arenaType._ArenaType__geometryCfg
            self.data = {
                'vehicles': {},
                'map': map_cfg['geometryName'],
            }
            self.map_bounds = map(list, map_cfg['boundingBox'])
        self._update_vehicle_basic_info(player)
        for vid, v in self.data['vehicles'].iteritems():
            v['accurate'] = v['active'] = False
            v['alive'] = vehicles[vid]['isAlive']
        for vid, pos in player.arena.positions.items():
            descr = self.data['vehicles'][vid]
            rpos = relative_position(pos, self.map_bounds)
            if 'dyn' in descr:
                descr['dyn']['pos'] = rpos
            else:
                descr['dyn'] = { 'pos': rpos }
            descr['active'] = True
        for v in map(BigWorld.entity, vehicles.keys()):
            if v is not None:
                try:
                    descr = self.data['vehicles'][v.id]
                    descr['dyn'] = get_vehicle_changes(v, self.map_bounds)
                    descr['active'] = True
                    descr['accurate'] = True
                except BigWorld.EntityIsDestroyedException:
                    pass
        return self.data

game_info = Info()

def send_info(stream):
    import json
    data = json.dumps(game_info.collect_data())
    stream.write(str(len(data)) + '\n')
    stream.write(data)

def send_map(stream, path, mapname):
    data = read_map(path, mapname)
    stream.write(str(len(data)) + '\n')
    stream.write(data)

def read_map(path, mapname):
    import zipfile
    with zipfile.ZipFile(os.path.join(path, mapname + '.pkg'), 'r') as zf:
        with zf.open('spaces/%s/mmap.dds' % mapname, 'r') as f:
            return f.read()

def run(res_path, address, port):
    def serve(stream):
        cmdmap = {
            'INFO': send_info,
            'MINIMAP': lambda s: send_map(s, res_path, s.readline().strip())
        }
        for cmd in stream:
            stream.write(cmd)
            cmdmap[cmd.strip()](stream)
            stream.flush()

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((address, port))
    s.listen(1)

    conn = f = None
    try:
        conn, addr = s.accept()
        f = conn.makefile()
        serve(f)
    except:
        import traceback
        print '****** map-viewer server crashed'
        print '#'*70
        traceback.print_exc()
    finally:
        s.close()
        if conn != None:
            conn.close()
            f.close()
