import argparse
import json
import sys

columns = 3

parser = argparse.ArgumentParser(description='Fake Debugger Prototype')
parser.add_argument("--out", type=argparse.FileType('w'),nargs="?",const=sys.stdout,help='The output file name')
parser.add_argument("jsonin", type=argparse.FileType('r'),help='The data for the page in json format')

args = parser.parse_args()

data = json.loads(args.jsonin.read())

# Make sure data goes evenly into columns
remainder = len(data)%columns
if remainder != 0:
  for i in range(columns - remainder):
    data.append({
      "title": "",
      "description": "",
    })


# print html
print('<table style="padding:10px">', file=args.out)
i = 0
while i < len(data):
  # Title row
  print('<tr>', file=args.out)
  for idx in range(i, i + columns):
    print('<td align="center" width={}%><br><h3>{}</h3></td>'.format(100/columns, data[idx]['title']), file=args.out)
  print('</tr>', file=args.out)

  # Description row
  print('<tr>', file=args.out)
  for idx in range(i, i + columns):
    link = ""
    if "more" in data[idx]:
      link = ' <a href="{}">More</a>'.format(data[idx]['more'])
    print('<td valign="top"><i>{}</i>{}</td>'.format(data[idx]['description'], link), file=args.out)
  print('</tr>', file=args.out)

  # Image row
  print('<tr>', file=args.out)
  for idx in range(i, i + columns):
    attrs = []
    if "image_width" in data[idx]:
      attrs.append("width={}".format(data[idx]['image_width']))
    if "image_height" in data[idx]:
      attrs.append("height={}".format(data[idx]['image_height']))

    img = ""
    if "image" in data[idx]:
      img = '<br><img {} src="{}"/>'.format(' '.join(attrs), data[idx]['image'])

    if img and "image_link" in data[idx]:
      img = '<a href="{}">{}</a>'.format(data[idx]['image_link'], img)

    print('<td align="center">{}</td>'.format(img), file=args.out)
  print('</tr>', file=args.out)
  i = i + columns

print('</table>', file=args.out)
