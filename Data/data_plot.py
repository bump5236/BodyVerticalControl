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
T_MIN = 10
T_MAX = 40
# -----------------------------------------------------------------------------

file1 = "9.30/20200930_4.csv"

# header = open(file,'r').readline().strip().split(',')
data = np.loadtxt(file1, delimiter=",", skiprows=1)

time = data[:, 0]
arduino_time = data[:, 1]
body_z = data[:, 2]
R_temp = data[:, 3]
R_add_cur = data[:, 4]
R_tgt_cur = data[:, 5]
R_cur = data[:, 6]
R_vel = data[:, 7]
R_pos = data[:, 8]

L_temp = data[:, 9]
L_add_cur = data[:, 10]
L_tgt_cur = data[:, 11]
L_cur = data[:, 12]
L_vel = data[:, 13]
L_pos = data[:, 14]


# 16bit Encoder
LSB = 65536
RGR = 6

R_torq = R_cur/2000*12.5*3.3
R_vel = R_vel*RGR
R_pos = R_pos/600
R_tgt_torq = R_tgt_cur/2000*12.5*3.3

L_torq = L_cur/2000*12.5*3.3
L_vel = L_vel*RGR
L_pos = L_pos/600
L_tgt_torq = L_tgt_cur/2000*12.5*3.3

plt.figure(figsize=(7,5))
plt.plot(time, R_tgt_torq, label= "R Target Torque", lw = LINE_WIDTH)
plt.plot(time, R_torq, label= "R Current Torque", lw = LINE_WIDTH)
plt.plot(time, L_tgt_torq, label= "L Target Torque", lw = LINE_WIDTH)
plt.plot(time, L_torq, label= "L Current Torque", lw = LINE_WIDTH)
plt.xlim(T_MIN, T_MAX)
plt.legend()

plt.figure(figsize=(7,5))
plt.plot(time, R_vel, label= "R Current Velocity", lw = LINE_WIDTH)
plt.plot(time, L_vel, label= "L Current Velocity", lw = LINE_WIDTH)
plt.xlim(T_MIN, T_MAX)
plt.legend()

plt.figure(figsize=(7,5))
plt.plot(time, R_pos, label= "R Current Poistion", lw = LINE_WIDTH)
plt.plot(time, L_pos, label= "L Current Poistion", lw = LINE_WIDTH)
plt.xlim(T_MIN, T_MAX)
plt.ylim(-100, 100)
plt.legend()

plt.figure(figsize=(7,5))
plt.plot(time, body_z, label= "body", lw = LINE_WIDTH)
plt.xlim(T_MIN, T_MAX)
plt.legend()

plt.show()