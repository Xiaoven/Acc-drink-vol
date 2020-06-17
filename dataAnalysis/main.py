#%%
import pandas as pd
import seaborn  as sns; sns.set()
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit
from scipy.misc import derivative
import math
from scipy.interpolate import make_interp_spline

#%%
# parameters
THRES_LOWPASS = 0.8    # 杯子静止时y轴加速度越在1左右，低通过滤，将数据切成不同片段
THRES_SEG_LEN = 10     # 数据片段最短长度，过滤放杯子时振荡产生的离群点 (尖锐的峰)
WIN_LEN = 10
DIFF_PERIOD = 8       # 越大 diff 曲线越平滑
THRES_RM_UP = -0.01
THRES_DEV = -0.02   # 临界喝水点加速度变化率阈值
#%%
def compute_tilt(x,y,z):
    return math.atan2( math.sqrt( pow(x,2) + pow(z,2)), y )*180/math.pi

#%%
def main(name):
    data = pd.read_csv(name,names=('acc_x', 'acc_y', 'acc_z'))
    acc = data['acc_y']

    # 低通过滤, 获得y轴acc喝水Pattern片段
    segments = []
    bool_index = (data['acc_y'] < THRES_LOWPASS) # type = Series <boolean>
    data['lowpass'] = bool_index.values
    group_key = (data['lowpass'] != data['lowpass'].shift()).cumsum()
    segments = [v for _,v in data.groupby(group_key) if v['lowpass'].values[0] and len(v.index) > THRES_SEG_LEN ] # list of dataframe

    #%%
    # 处理每一个喝水segment
    for segment in segments:
        # 平滑抖动:简单移动平均线
        weights = np.ones(WIN_LEN)/WIN_LEN
        smooth_data = np.convolve(weights, segment['acc_y'].values, mode='valid') # 返回长度 abs(m - n) + 1
        smooth_data_df = pd.DataFrame(smooth_data, columns=['acc_y'])
        smooth_data_df.index += segment['acc_y'].index[0] + int(WIN_LEN/2) # 保持与 segment 样本编号一致
        # 插值
        x_new = np.linspace(smooth_data_df.index[0], smooth_data_df.index[-1], 2*len(smooth_data_df.index))
        y_smooth = make_interp_spline(smooth_data_df.index, smooth_data_df['acc_y'].values)(x_new)
        intplt_df = pd.DataFrame(y_smooth, columns=['acc_y'])
        intplt_df.index = x_new

        # 对平滑后对数据做差分
        diffs = smooth_data_df.diff(periods=DIFF_PERIOD)
        diffs = diffs.drop(diffs.index[:DIFF_PERIOD]) # 删除前DIFF_PERIOD行的NAn
        diffs.columns=['diff']

        # 求角度
        tilt_angles = [ compute_tilt(row['acc_x'],row['acc_y'],row['acc_z']) for _,row in segment.iterrows()]
        angles_df = pd.DataFrame(tilt_angles, columns=['angle'])
        angles_df.index = segment.index
        # 平滑角度
        weights = np.ones(WIN_LEN)/WIN_LEN
        smooth_angles = np.convolve(weights, angles_df['angle'].values, mode='valid') # 返回长度 abs(m - n) + 1
        smooth_angle_df = pd.DataFrame(smooth_data, columns=['angle'])
        smooth_angle_df.index += angles_df['angle'].index[0] + int(WIN_LEN/2) # 保持与 segment 样本编号一致

    #%%
        critical_idx = next(i for i in diffs.index if diffs['diff'][i] >= THRES_DEV)
        critical_idx = int(critical_idx)

        diffs = diffs.sort_index(ascending=False)
        end_idx = next(i for i in diffs.index if diffs['diff'][i] <= 0)
        end_idx = int(end_idx)

    #%%
        print(smooth_angle_df['angle'][critical_idx], smooth_angle_df['angle'][end_idx])
        print(angles_df['angle'][critical_idx] * 180/math.pi, angles_df['angle'][end_idx] * 180/math.pi)
        # print('------------')
    #%%
        #%% 画图
        fig,axes = plt.subplots(3,1,figsize=(30,24))
        start,end = segment.index[0], segment.index[-1]
        axes[0].set_xlim([start, end])
        axes[1].set_xlim([start, end])
        axes[2].set_xlim([start, end])
        sns.lineplot(data=smooth_data_df,ax=axes[0])
        sns.lineplot(data=segment['acc_y'],ax=axes[0])
        # sns.lineplot(data=intplt_df, ax=axes[0])
        sns.lineplot(data=diffs, ax=axes[1])
        sns.lineplot(data=angles_df, ax=axes[2])

        points = pd.DataFrame([[critical_idx, segment['acc_y'][critical_idx]],[end_idx,segment['acc_y'][end_idx]]],
                columns=['index','acc_y'])
        sns.scatterplot(x='index',y='acc_y',data=points, ax=axes[0])
        points2 = pd.DataFrame([[critical_idx, angles_df['angle'][critical_idx]],[end_idx,angles_df['angle'][end_idx]]],
                columns=['index','angle'])
        sns.scatterplot(x='index',y='angle',data=points2)

        fig.savefig(name.replace('.txt', '.png'))
# %
if __name__ == "__main__":
    files=['demo/a_drink.txt','demo/a_long_drink.txt','demo/a_quick_drink.txt','demo/three_drink.txt']
    for fname in files:
        main(fname)