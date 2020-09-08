# -*- coding: utf-8 -*-

import sys
import cmath
import math

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import matplotlib.ticker as tick


#グラフの書式設定 -------------------------------------------------------------
plt.rcParams["font.family"] = "Arial"                #全体のフォントを設定
plt.rcParams['mathtext.fontset'] = 'cm'              #数式のフォントを設定
plt.rcParams["font.size"] = 12                       #フォントの大きさ
plt.rcParams['axes.labelsize'] = 16                  #ラベルのフォントの大きさ
plt.rcParams["xtick.direction"] = "in"               #x軸の目盛線が内向き('in')か外向き('out')か双方向か('inout')
plt.rcParams["ytick.direction"] = "in"               #y軸の目盛線が内向き('in')か外向き('out')か双方向か('inout')
plt.rcParams["xtick.minor.visible"] = False          #x軸補助目盛りがあり(True)かなし(False)か
plt.rcParams["ytick.minor.visible"] = False          #y軸補助目盛りがあり(True)かなし(False)か
plt.rcParams["xtick.major.width"] = 1.0              #x軸主目盛り線の線幅
plt.rcParams["ytick.major.width"] = 1.0              #y軸主目盛り線の線幅
plt.rcParams["xtick.minor.width"] = 1.0              #x軸補助目盛り線の線幅
plt.rcParams["ytick.minor.width"] = 1.0              #y軸補助目盛り線の線幅
plt.rcParams["xtick.major.size"] = 6                 #x軸主目盛り線の長さ
plt.rcParams["ytick.major.size"] = 6                 #y軸主目盛り線の長さ
plt.rcParams["xtick.minor.size"] = 5                 #x軸補助目盛り線の長さ
plt.rcParams["ytick.minor.size"] = 5                 #y軸補助目盛り線の長さ
plt.rcParams["axes.linewidth"] = 1.0                 #囲みの太さ

LINE_WIDTH = 0.5
# -----------------------------------------------------------------------------

file1 = "output.csv"

# header = open(file,'r').readline().strip().split(',')
data = np.loadtxt(file1, delimiter=",", skiprows=1)

time = data[:, 0]
arduino_time = data[:, 1]
tgt_pos = data[:, 2]
cmd_byte = data[:, 3].astype("int8")
temperature = data[:, 4].astype("uint8")
cur_L = data[:, 5].astype("uint8")
cur_H = data[:, 6].astype("uint8")
vel_L = data[:, 7].astype("uint8")
vel_H = data[:, 8].astype("uint8")
enc_L = data[:, 9].astype("uint8")
enc_H = data[:, 10].astype("uint8")
pos_cmd = data[:, 11].astype("uint8")
pos_low_1 = data[:, 12].astype("uint8")
pos_2 = data[:, 13].astype("uint8")
pos_3 = data[:, 14].astype("uint8")
pos_4 = data[:, 15].astype("uint8")
pos_5 = data[:, 16].astype("uint8")
pos_6 = data[:, 17].astype("uint8")
pos_7 = data[:, 18].astype("uint8")

rcur = (cur_H.astype("int16") << 8) + cur_L
rvel = (vel_H.astype("int16") << 8) + vel_L
renc = (enc_H.astype("int16") << 8) + enc_L

# 16bit Encoder
LSB = 65536
RGR = 6

tgt_pos = tgt_pos/100/RGR
cur = rcur/2048*33
vel = rvel*RGR
pos = renc/LSB*360/RGR

plt.figure(figsize=(7,5))
plt.plot(time, cur, label= "current", lw = LINE_WIDTH)

plt.figure(figsize=(7,5))
plt.plot(time, vel, label= "velocity", lw = LINE_WIDTH)

plt.figure(figsize=(7,5))
plt.plot(time, tgt_pos, label= "input", lw = LINE_WIDTH)
plt.plot(time, pos, label= "output", lw = LINE_WIDTH)

plt.legend()
plt.show()