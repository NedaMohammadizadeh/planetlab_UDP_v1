import csv
import sys
import glob, os
from io import StringIO
#from numpy import genfromtxt as gntxt
import numpy as np
import pandas as pd
import string
import decimal
pd.options.display.float_format = '{:.9f}'.format
# pd.set_option=('float_precision', 9)


# data = {}
# count = 0
#
# def find(reader, col, val):
#     for row in reader.itertuples(index=False):
#         if row[col] == val:
#             count = count
#         else:
#             count = count+1
#             data.append(reader._currow)
#
#
# def findexcept (reader, col, val):
#     r = find(reader, col, val)
#     r.pop(col)
#     return r

# if (len(sys.argv) < 4):
    # raise Exception('Number of arguments are less than 3\n',
    # 'Usage: csvparser.py [Outfile Location] [Infile Location] [Time Delta File]' )

#Get and verify the file name and locations
user_input_out = sys.argv[1]
# user_input_out = "/home/ttadayon/wireshark/june04/rt3_ethz_jun04_2359/send_rt3_ethz_jun04_2359_01.txt"
assert os.path.exists(user_input_out), "I did not find the input at, "+str(user_input_out)
# user_input_in = "/home/ttadayon/wireshark/june04/rt3_ethz_jun04_2359/recv_rt3_ethz_jun04_2359_01.txt"
user_input_in = sys.argv[2]
assert os.path.exists(user_input_in), "I did not find the input at, "+str(user_input_in)

# datadump = sys.argv[3]
timedeltafile = sys.argv[3]
# timedeltafile = "/home/ttadayon/wireshark/june04/rt3_ethz_jun04_2359/delta_01.csv"

#open file and create reader
outf = pd.read_csv(user_input_out, delimiter = '\t', header=0, dtype = object, float_precision = 'high')
inf = pd.read_csv(user_input_in, delimiter = '\t', header=0, dtype = object, float_precision= 'high')

tcount=0
reordered=0
dropped = 0
rowout = 0
rowin = 0
# outf['frame.time'] = pd.to_datetime(outf['frame.time'], format="%b %d, %Y %H:%M:%S.%f %Z")
# inf['frame.time'] = pd.to_datetime(inf['frame.time'], format="%b %d, %Y %H:%M:%S.%f %Z")


cols = outf.columns
cols = cols.map(lambda x: x.replace('.', '_') if isinstance(x, str) else x)
# outf['data'] = outf['data'].map(lambda x: x.replace(' ', ''))
outf.columns = cols

cols = inf.columns
cols = cols.map(lambda x: x.replace('.', '_') if isinstance(x, str) else x)
inf.columns = cols


# outf['frame_time_epoch'] = outf['frame_time_epoch'].astype(float64)

outf['data'] = outf['data'].astype(str)
outf['data'] = outf['data'].str[:-2:]
outf['data'] = outf['data'].map(lambda x: bytearray.fromhex(x).decode())
outf['data'] = outf['data'].astype(int)


inf['data'] = inf['data'].astype(str)
inf['data'] = inf['data'].str[:-2]
inf['data'] = inf['data'].map(lambda x: bytearray.fromhex(x).decode())
inf['data'] = inf['data'].astype(int)

inf.columns = ['data', 'recv_time_epoch']
outf.columns = ['data', 'send_time_epoch']
inf.sort_values(by='data', inplace=True, kind='quicksort')


assert inf.data.is_monotonic_increasing


rng = range(outf.data.iloc[0], outf.data.iloc[-1]+1, 1)
deltadf = pd.DataFrame(index=rng, columns=["send_time_epoch", "recv_time_epoch"])

deltadf.update(inf.set_index("data"))
deltadf.update(outf.set_index("data"))
dropped = deltadf['recv_time_epoch'].isnull().sum().sum()
#find the difference of the two columns
deltadf['delta'] = deltadf['recv_time_epoch'].astype(np.longdouble)-deltadf['send_time_epoch'].astype(np.longdouble)
# timedelta = timedelta.append(tempp, ignore_index=True)

# outf.reset_index(drop=True)
# outf.to_csv(datadump, sep="\t")
deltadf.fillna('NaN')
deltadf.to_csv(timedeltafile, sep=",", float_format='%.9F')
# print(outf, '\n')
# print(timedelta, '\n')
print('Dropped = ', dropped, 'reordered = ', reordered, 'rowout = ', len(outf), 'rowin = ', len(inf), '\n')




# print(tcount, '\n')
print('outfile\n')



# print(outf.columns)
# print(len(outf.columns))
# print(outf.shape)
# print(outf.columns)
#
# print('infile\n')
# print(len(inf.columns))
# print(inf.shape)
#
# print(inf.columns)

# print(outf.loc[:,'data.data'])





#print(outfile.shape[1])
# print(outf[1][:])
