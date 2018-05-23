clc; clear;

% add yaml lib to matlab path
addpath(genpath('YAMLMatlab_0.4.3')); 

% Path
test = 'against_per.csv';
path = strcat('../tests/', test);

% get csv file
M = csvread(path);

% params
game_test = 10000;

games_winrate = M(:,1)/game_test; 
p0_mean = mean(games_winrate)
p0_std = var(games_winrate)

p1_wins = M(:,2)/game_test;
p1_mean = mean(p1_wins)
p1_std = std(p1_wins)

% hypotese test
normplot(M(:,1));

% test that test2 mean is higher than other mean
[h,p] = ttest(M(:,1),M(:,2),'Tail','right','Alpha', 0.01)

% p is increadbly small == very strong rejection at 1% signifigance