//getting url for tile server
//code src: https://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#C/C++
//tile servers: https://{a,b,c}.tile.openstreetmap.org/{z}/{x}/{y}.png
#include<stdio.h>
#include<math.h>
#include<stdlib.h> //atoi, ftoi

int long2tilex(double lon, int z) 
{ 
	return (int)(floor((lon + 180.0) / 360.0 * (1 << z))); 
}

int lat2tiley(double lat, int z)
{ 
    double latrad = lat * M_PI/180.0;
	return (int)(floor((1.0 - asinh(tan(latrad)) / M_PI) / 2.0 * (1 << z))); 
}

double tilex2long(int x, int z) 
{
	return x / (double)(1 << z) * 360.0 - 180;
}

double tiley2lat(int y, int z) 
{
	double n = M_PI - 2.0 * M_PI * y / (double)(1 << z);
	return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

int valid_check(int ac, char **av)
{
	if (ac != 4)
    {
        printf("Usage: ./exec zoom lon lat\n");
        return (1);
    }

	if (atoi(av[1]) < 0 || atoi(av[1]) > 22)
    {
        printf("Error! Invalid zoom value! %d no in [0;22]\n", atoi(av[1]));
        return (2);
    }

	if (atof(av[2]) < -180.0 || atof(av[2]) > 180.0)
    {
        printf("Error! Invalid longitude value! %f not in [-180.0;180.0]\n", atof(av[2]));
        return (2);
    }

    if (atof(av[3]) < -90.0 || atof(av[3]) > 90.0)
    {
        printf("Error! Invalid latitude value! %f not in [-90.0;90.0]\n", atof(av[3]));
        return (2);
    }

	return (0);
}

int main(int ac, char **av)
{
	int		x;
	int		y;
	double	lon_left;
	double	lon_right;
	double	lat_up;
	double	lat_bottom;
	int		valid_result;
	const int marker_height = 45;
	const int marker_width = 25;

	valid_result = valid_check(ac, av);
	if (valid_result)
		return (valid_result);
	x = long2tilex( atof(av[2]), atoi(av[1]) );
	y = lat2tiley( atof(av[3]), atoi(av[1]) );
	lon_left = tilex2long(x, atoi(av[1]));
	lon_right = tilex2long(x + 1, atoi(av[1]));
	lat_up = tiley2lat(y, atoi(av[1]));
	lat_bottom = tiley2lat(y + 1, atoi(av[1]));

	printf("zoom = %d, longitude = %f, latitude = %f\n",
		atoi(av[1]), atof(av[2]), atof(av[3]));
	printf("tiles upper bound = %f, bottom bound = %f, left bound = %f, right bound = %f\n", lat_up, lat_bottom, lon_left, lon_right);
	printf("\tOSM 'standard' style: https://tile.openstreetmap.org/%d/%d/%d.png\n", atoi(av[1]), x, y);
	printf("\tmarker's position: horizontal offset = %f, vertical offset = %f",
		fabs( lon_left - atof(av[2]) )/ fabs( lon_right - lon_left ) * 256 - marker_width/2,
		fabs( lat_up - atof(av[3]) )  / fabs( lat_up - lat_bottom ) * 256 - marker_height
	);
	return (0);
}
