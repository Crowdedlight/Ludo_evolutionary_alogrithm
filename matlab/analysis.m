clc; clear;

% add yaml lib to matlab path
addpath(genpath('YAMLMatlab_0.4.3')); 

% Path
path = '../generations/';

%get subdirectories
directories = dir(path);
directories=directories(~ismember({directories.name},{'.','..'}));
% Get a logical vector that tells which is a directory.
dirFlags = [directories.isdir];
% Extract only those that are directories.
subFolders = directories(dirFlags);

% make 2-dimensional vector
winrate_top = zeros(length(subFolders));

figure;
hold on;

for ds = subFolders'
    
    % get files from subfolder   
    files = dir(strcat(path, ds.name, '/*.yaml'));
    [~, reindex] = sort( str2double( regexp( {files.name}, '\d+', 'match', 'once' )));
    files = files(reindex);

    % parameters
    winrate = zeros(1,length(files));
    winrate_avg = zeros(1,length(files));
    i = 1;

    for file = files'
        filePath = strcat(file.folder, '/', file.name);
        YamlStruct = ReadYaml(filePath);

        %take first population with best winrate and save for plot
        best = YamlStruct.population{1};
        winrate(i) = best.win_rate;   
        wra = 0;
        for j = 1 : 50
            wra = wra + YamlStruct.population{j}.win_rate;
        end
        winrate_avg(i) = wra/50;
        
        i = i+1;
    end
    
    plot(winrate);
    plot(winrate_avg);

end