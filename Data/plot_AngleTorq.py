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

file1 = "10.12/output0.csv"
file2 = "Running/2020100800002.csv"

# header = open(file,'r').readline().strip().split(',')
data = np.loadtxt(file1, delimiter=",", skiprows=1)
Run_data = np.loadtxt(file2, delimiter=",", skiprows=1)

time = data[:, 0]
arduino_time = data[:, 1]
sync = data[:, 2]
R_temp = data[:, 3]
R_add_cur = data[:, 4]
R_tgt_cur = data[:, 5]
R_cur = data[:, 6]
R_vel = data[:, 7]
R_ang = data[:, 8]
R_mode = data[:, 9]

L_temp = data[:, 10]
L_add_cur = data[:, 11]
L_tgt_cur = data[:, 12]
L_cur = data[:, 13]
L_vel = data[:, 14]
L_ang = data[:, 15]
L_mode = data[:, 16]


run_time = Run_data[:, 0]
run_tgt_cur = Run_data[:, 1]
run_pre_cur = Run_data[:, 3]
run_pre_vel = Run_data[:, 4]
run_ang = Run_data[:, 5]
run_tgt_torq = Run_data[:, 6]
run_mode = Run_data[:, 7]


# 16bit Encoder
LSB = 65536
RGR = 6

R_torq = R_cur/2000*12.5*3.3
R_vel = R_vel*RGR
R_tgt_torq = R_tgt_cur/2000*12.5*3.3

L_torq = L_cur/2000*12.5*3.3
L_vel = L_vel*RGR
L_tgt_torq = L_tgt_cur/2000*12.5*3.3

l = 1

plt.figure(figsize=(10,8))
plt.plot(R_ang, R_tgt_torq/l, label= "R Target Torque", lw = LINE_WIDTH)
# plt.plot(R_ang, R_torq/l, label= "R Current Torque", lw = LINE_WIDTH)
# plt.xlim(T_MIN, T_MAX)
plt.legend()

plt.figure(figsize=(10,8))
# plt.plot(run_ang+5, run_tgt_torq/0.5*0.4/l, label= "Run Target Torque", lw = LINE_WIDTH)
plt.plot(L_ang, L_tgt_torq/l, label= "L Target Torque", lw = LINE_WIDTH)
# plt.plot(L_ang, L_torq/l, label= "L Current Torque", lw = LINE_WIDTH)
# plt.xlim(T_MIN, T_MAX)
plt.legend()


# plt.figure(figsize=(10,8))
# plt.plot(run_ang, run_tgt_torq/0.5*1.05, label= "Run Target Torque", lw = LINE_WIDTH)
# plt.plot(run_ang, run_pre_cur/2000*12.5*3.3*2.5, label= "Run Current Torque", lw = LINE_WIDTH)
# # plt.xlim(T_MIN, T_MAX)
# plt.legend()

# s = 100
# g = 5000
# plt.figure(figsize=(10,8))
# plt.plot(run_time[s:g]/1000, run_tgt_torq[s:g]/0.5*0.4, label= "Run Target Torque", lw = LINE_WIDTH)
# plt.plot(time[s:g], L_tgt_torq[s:g], label= "L Target Torque", lw = LINE_WIDTH)
# plt.plot(time[s:g], L_torq[s:g], label= "L Current Torque", lw = LINE_WIDTH)
# # plt.xlim(T_MIN, T_MAX)
# plt.legend()


plt.show()