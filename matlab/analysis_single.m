clc; clear;

% add yaml lib to matlab path
addpath(genpath('YAMLMatlab_0.4.3')); 

% Path
population = 'pop20-1000games-2mutation-2range-std-player-4tournement-71winrate';
path = strcat('../generations/', population);

% get files from subfolder   
files = dir(strcat(path, '/*.yaml'));
[~, reindex] = sort( str2double( regexp( {files.name}, '\d+', 'match', 'once' )));
files = files(reindex);

% parameters
winrate = zeros(1,length(files));
winrate_avg = zeros(1,length(files));
i = 1;
pop_size = 20;

% boxplot
all_winrates = zeros(length(files), pop_size);
best_avg = zeros(2, pop_size);

% get data
for file = files'
    filePath = strcat(file.folder, '/', file.name);
    YamlStruct = ReadYaml(filePath);

    %entire population winrate
    tempM = zeros(1, pop_size);
    tempM = cell2mat(YamlStruct.win0x2Drate);
    
    all_winrates(i,:) = tempM;

    %take first population with best winrate and save for plot
    best = YamlStruct.population{1};
    best_avg(i,1) = best.win_rate;   
    wra = 0;
    for j = 1 : pop_size
        wra = wra + YamlStruct.population{j}.win_rate;
    end
    best_avg(i, 2) = wra/pop_size;

    i = i+1;
end
    
figure;
hold on
%Axises
xlabel('Generation');
ylabel('Win Rate');

plot(best_avg(:,1)*100);
plot(best_avg(:,2)*100);   

% sort out legend
Legend=cell(2,1);
Legend{1}=strcat('best-', population);
Legend{2}=strcat('avg-', population);

legend(Legend,'Location','southeast');
    
hold off;

figure;
% axis
xlabel('Generation');
ylabel('Win Rate');
% trim data, to actual show intersting parts
trimmedData = all_winrates(1:1700,:);
every_ten_gen = all_winrates(20:20:end,:);
boxplot(transpose(every_ten_gen),'PlotStyle','compact')
    