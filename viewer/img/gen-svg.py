image_mapping = {
    'cancer': 'SPG',
    'td': 'AT-SPG',
    'tlt': 'lightTank',
    'tmt': 'mediumTank',
    'tht': 'heavyTank',
}

targets = [
    ('fa', '#00ff00'),
    ('ea', '#ff0000'),
    ('fi', '#00aa00'),
    ('ei', '#aa0000'),
]

for src, wotname in image_mapping.iteritems():
    for postfix, color in targets:
        dst_name = '%s_%s.svg'%(wotname, postfix)
        with open('%s.svg' % src) as sf, open(dst_name, 'w') as df:
            for line in sf:
                df.write(line.replace(
                    'style="fill:#00ff00"',
                    'style="fill:%s"' % color))
