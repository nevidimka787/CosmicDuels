#include "NetPoint.h"

#pragma warning (disable : 6011)
#pragma warning (disable : 6308)
#pragma warning (disable : 6386)
#pragma warning (disable : 28182)

NetPoint::NetPoint()
	:
	DynamicEntity(),
	exist_time(0),
	connected_points_ps((NetPoint**)malloc(sizeof(NetPoint*))),
	connected_points_array_length(0),
	connected_points_count(0),
	host_number(0),
	host_team_number(0)
{
	connected_points_ps[0] = nullptr;
}

NetPoint::NetPoint(const NetPoint& net_point)
	:
	DynamicEntity(position, velocity, 0.0f),
	exist_time(net_point.exist_time),
	connected_points_ps((net_point.connected_points_count == 0) ? (NetPoint**)malloc(sizeof(NetPoint*)) : (NetPoint**)malloc(sizeof(NetPoint*) * net_point.connected_points_count)),
	connected_points_array_length(net_point.connected_points_array_length ? net_point.connected_points_array_length : 1),
	connected_points_count(net_point.connected_points_count),
	host_number(net_point.host_number),
	host_team_number(net_point.host_team_number)
{
	if (connected_points_count == 0)
	{
		connected_points_ps[0] = nullptr;
		return;
	}
	for (id_t point = 0; point < connected_points_count; point++)
	{
		connected_points_ps[point] = net_point.connected_points_ps[point];
	}
}

NetPoint::NetPoint(
	Vec2F position,
	Vec2F velocity,
	GameTypes::tic_t exist_time,
	GameTypes::players_count_t host_number,
	GameTypes::players_count_t host_team_number,
	NetPoint** connected_points_pointers,
	id_t connected_points_count)
	:
	DynamicEntity(position, velocity, 0.0f),
	exist_time(exist_time),
	connected_points_ps((connected_points_count == 0) ? (NetPoint**)malloc(sizeof(NetPoint*)) : (NetPoint**)malloc(sizeof(NetPoint*) * connected_points_count)),
	connected_points_array_length(connected_points_count ? connected_points_count : 1),
	connected_points_count(connected_points_count),
	host_number(host_number),
	host_team_number(host_team_number)
{
	if (connected_points_count == 0)
	{
		connected_points_ps[0] = nullptr;
		return;
	}
	for (id_t point = 0; point < connected_points_count; point++)
	{
		connected_points_ps[point] = connected_points_pointers[point];
	}
}

NetPoint::NetPoint(
	const Vec2F* position,
	const Vec2F* velocity,
	GameTypes::tic_t exist_time,
	GameTypes::players_count_t host_number,
	GameTypes::players_count_t host_team_number,
	NetPoint** connected_points_pointers,
	id_t connected_points_count)
	:
	DynamicEntity(position, velocity, 0.0f),
	exist_time(exist_time),
	connected_points_ps((connected_points_count == 0) ? (NetPoint**)malloc(sizeof(NetPoint*)) : (NetPoint**)malloc(sizeof(NetPoint*) * connected_points_count)),
	connected_points_array_length(connected_points_count ? connected_points_count : 1),
	connected_points_count(connected_points_count),
	host_number(host_number),
	host_team_number(host_team_number)
{
	if (connected_points_count == 0)
	{
		connected_points_ps[0] = nullptr;
		return;
	}
	for (id_t point = 0; point < connected_points_count; point++)
	{
		connected_points_ps[point] = connected_points_pointers[point];
	}
}

NetPoint::NetPoint(
	Vec2F position,
	Vec2F velocity,
	GameTypes::tic_t exist_time,
	GameTypes::players_count_t host_number,
	GameTypes::players_count_t host_team_number,
	NetPoint* connected_points_array,
	const id_t* connected_points_ids_in_array,
	id_t ids_count)
	:
	DynamicEntity(position, velocity, 0.0f),
	exist_time(exist_time),
	connected_points_ps((ids_count == 0) ? (NetPoint**)malloc(sizeof(NetPoint*)) : (NetPoint**)malloc(sizeof(NetPoint*) * ids_count)),
	connected_points_array_length(ids_count ? ids_count : 1),
	connected_points_count(ids_count),
	host_number(host_number),
	host_team_number(host_team_number)
{
	if (connected_points_count == 0)
	{
		connected_points_ps[0] = nullptr;
		return;
	}
	for (id_t point = 0; point < connected_points_count; point++)
	{
		connected_points_ps[point] = &connected_points_array[connected_points_ids_in_array[point]];
	}
}

NetPoint::NetPoint(
	const Vec2F* position,
	const Vec2F* velocity,
	GameTypes::tic_t exist_time,
	GameTypes::players_count_t host_number,
	GameTypes::players_count_t host_team_number,
	NetPoint* connected_points_array,
	const id_t* connected_points_ids_in_array,
	id_t ids_count)
	:
	DynamicEntity(position, velocity, 0.0f),
	exist_time(exist_time),
	connected_points_ps((ids_count == 0) ? (NetPoint**)malloc(sizeof(NetPoint*)) : (NetPoint**)malloc(sizeof(NetPoint*) * ids_count)),
	connected_points_array_length(ids_count ? ids_count : 1),
	connected_points_count(ids_count),
	host_number(host_number),
	host_team_number(host_team_number)
{
	if (connected_points_count == 0)
	{
		connected_points_ps[0] = nullptr;
		return;
	}
	for (id_t point = 0; point < connected_points_count; point++)
	{
		connected_points_ps[point] = &connected_points_array[connected_points_ids_in_array[point]];
	}
}

void NetPoint::AddPoint(NetPoint* connected_point)
{
	if (connected_points_count == connected_points_array_length)
	{
		connected_points_array_length++;
		connected_points_ps = (NetPoint**)realloc(connected_points_ps, sizeof(NetPoint*) * connected_points_array_length);

		connected_points_ps[connected_points_count] = connected_point;
		connected_points_count++;

		return;
	}

	for (id_t point = 0; point < connected_points_array_length; point++)
	{
		if (connected_points_ps[point] == nullptr)
		{
			connected_points_ps[point] = connected_point;
			connected_points_count++;
			return;
		}
	}
}

void NetPoint::ClearMemory()
{
	id_t clear_size = 0;
	for (id_t point = connected_points_array_length - 1; point > 0; point--)
	{
		if (connected_points_ps[point] == nullptr)
		{
			clear_size++;
		}
		else
		{
			if (clear_size == 0)
			{
				return;
			}

			connected_points_array_length -= clear_size;
			connected_points_ps = (NetPoint**)realloc(connected_points_ps, sizeof(NetPoint*) * connected_points_array_length);

			return;
		}
	}
}

void NetPoint::ConnectTo(NetPoint* connected_point)
{
	AddPoint(connected_point);
}

void NetPoint::ConnectTo(
	NetPoint* connected_points_array,
	id_t points_count)
{
	for (id_t point = 0; point < points_count; point++)
	{
		AddPoint(&connected_points_array[point]);
	}
}

void NetPoint::ConnectTo(
	NetPoint* connected_points_array,
	const id_t* connected_points_ids_in_array,
	id_t ids_count)
{
	for (id_t point = 0; point < ids_count; point++)
	{
		AddPoint(&connected_points_array[connected_points_ids_in_array[point]]);
	}
}

void NetPoint::DesegnemtationMemory()
{
	NetPoint** new_arr = (NetPoint**)malloc(sizeof(NetPoint*) * (connected_points_count ? connected_points_count : 1));
	new_arr[0] = nullptr;
	for (id_t point = 0, new_point = 0; new_point < connected_points_count; point++)
	{
		if (connected_points_ps[point] != nullptr)
		{
			new_arr[new_point] = connected_points_ps[point];
			new_point++;
		}
	}
	free(connected_points_ps);
	connected_points_ps = new_arr;
}

void NetPoint::DisConnetc(NetPoint* connected_point)
{
	RemovePoint(connected_point);
}

void NetPoint::DisConnetc(
	NetPoint* connected_points_array,
	id_t points_count)
{
	for (id_t point = 0; point < points_count; point++)
	{
		RemovePoint(&connected_points_array[point]);
	}
}

void NetPoint::DisConnetc(
	NetPoint* connected_points_array,
	const id_t* connected_points_ids_in_array,
	id_t ids_count)
{
	for (id_t point = 0; point < ids_count; point++)
	{
		RemovePoint(&connected_points_array[connected_points_ids_in_array[point]]);
	}
}

void NetPoint::DisConnectAll()
{
	connected_points_array_length = 0;
	connected_points_count = 0;
	free(connected_points_ps);
}

NetPoint NetPoint::GetConnectedPoint(id_t point_id)
{
	return (connected_points_ps[point_id] == nullptr) ? NetPoint() : *connected_points_ps[point_id];
}

NetPoint* NetPoint::GetConnectedPointP(id_t point_id)
{
	return connected_points_ps[point_id];
}

NetPoint* NetPoint::GetConnectedPoints()
{
	NetPoint* return_array = new NetPoint[connected_points_count];
	for (id_t point_p = 0, point = 0; point < connected_points_count; point_p++)
	{
		if (connected_points_ps[point_p] != nullptr)
		{
			return_array[point] = *connected_points_ps[point_p];
		}
	}

	return return_array;
}

NetPoint** NetPoint::GetConnectedPointsPs()
{
	NetPoint** return_array = new NetPoint*[connected_points_count];
	for (id_t point_p = 0, point = 0; point < connected_points_count; point_p++)
	{
		if (connected_points_ps[point_p] != nullptr)
		{
			return_array[point] = connected_points_ps[point_p];
		}
	}

	return return_array;
}


bool NetPoint::IsConnected(const NetPoint* connected_point)
{
	if (connected_point == nullptr)
	{
		return false;
	}

	for (id_t point = 0; point < connected_points_array_length; point++)
	{
		if (connected_point == connected_points_ps[point])
		{
			return true;
		}
	}

	return false;
}

void NetPoint::RemovePoint(id_t point_id)
{
	if (connected_points_ps[point_id] != nullptr)
	{
		connected_points_ps[point_id] == nullptr;
		connected_points_count--;
	}
}

void NetPoint::RemovePoint(NetPoint* point_p)
{
	if (point_p == nullptr)
	{
		return;
	}
	for (id_t point = 0; point < connected_points_array_length; point++)
	{
		if (connected_points_ps[point] == point_p)
		{
			connected_points_ps[point] = nullptr;
			connected_points_count--;
			return;
		}
	}
}

void NetPoint::UpdateConnections()
{
	for (id_t point = 0, point_p = 0; point < connected_points_count; point_p++)
	{
		if (connected_points_ps[point_p] != nullptr)
		{
			if (connected_points_ps[point_p]->exist_time == 0)
			{
				RemovePoint(point_p);
			}
			else
			{
				point++;
			}
		}
	}
}

void NetPoint::operator=(NetPoint net_point)
{
	exist = net_point.exist;
	exist_time = net_point.exist_time;
	connected_points_array_length = net_point.connected_points_array_length;
	connected_points_count = net_point.connected_points_count;
	connected_points_ps = (NetPoint**)realloc(connected_points_ps, sizeof(NetPoint*) * net_point.connected_points_array_length);
	position = net_point.position;
	velocity = net_point.velocity;

	for (id_t point = 0; point < connected_points_array_length; point++)
	{
		connected_points_ps[point] = net_point.connected_points_ps[point];
	}
}

NetPoint::~NetPoint()
{
	if (connected_points_array_length != 0)
	{
		free(connected_points_ps);
	}
}