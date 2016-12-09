img = imread('cabinet_depth.png');

figure(1);
imshow(img);
colormap jet;

fx=535.4;
fy=539.2;
cx=320.1;
cy=247.6;

factor=5000.0;

points = zeros(3,sum(size(img)));
for v = 1:size(img, 1)
  for u = 1:size(img,2)
    Z = double(img(v,u)) / factor;
    X = (u - cx) * Z / fx;
    Y = (v - cy) * Z / fy;
    points(:,v*size(img,2) + u) = [X Y Z];
  end;
end;

points2 = [points(1,:)',points(3,:)',-points(2,:)'];
figure(2);
pcshow(points2);
xlabel('X');
ylabel('Y');
zlabel('Z');
axis vis3d;

