fil = fopen('output.txt','r');
data=fscanf(fil,'%f,%f,%f,%f', [4 Inf]); 
data=data';
fclose(fil);

points=[data(:,1), data(:,3), -data(:,2)];
labels=data(:,4);

colors = zeros(size(data,1), 3);
colors(labels == 1,1) = 1;
colors(labels == 2,2) = 1;
colors(labels == 3,3) = 1;
colors(labels == 4,1) = 1;
colors(labels == 4,2) = 1;



figure(1);
pcshow(points, colors, 'Markersize', 50);
xlabel('X');
ylabel('Y');
zlabel('Z');
axis vis3d;

