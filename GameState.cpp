#include "GameState.h"

GameState::GameState(Player *player)
{
	p = player;
	a = true;
	b = true;
	up = true;
	down = true;
	left = true;
	right = true;
}

void GameState::startMenu()
{

}

void GameState::endMenu()
{
	a = keys[p->keyA];
	b = keys[p->keyB];
	up = keys[p->keyUp];
	down = keys[p->keyDown];
	left = keys[p->keyLeft];
	right = keys[p->keyRight];
}

WorldState::WorldState(Player *player) : GameState(player)
{

}

void WorldState::draw()
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0);
	SDL_RenderClear(renderer);
	World *world = worlds[p->worldID];
	for (int i = p->x - WIDTH / 32 / 2 - 2; i <= p->x + WIDTH / 32 / 2 + 2; i++)
	{
		for (int j = p->y - HEIGHT / 64 - 2; j <= p->y + HEIGHT / 64 + 2; j++)
		{
			int index = world->getLower(i, j);
			if (tileset[index] != nullptr&&index!=0) tileset[index]->draw(WIDTH / 2 - 16 - p->xOffset + 32 * (i - p->x), HEIGHT / 2 - 16 - p->yOffset + 32 * (j - p->y));
		}
	}
	List<Entity*> &entities = worlds[p->worldID]->entities;
	for (int i = 0; i < entities.length(); i++)
	{
		Entity *e = entities[i];
		assert(e != nullptr);
		if (e->isAlive)
		{
			if(e->sprite!=nullptr) e->sprite->draw(getOnscreenX(p, e->x, e->xOffset), getOnscreenY(p, e->y, e->yOffset));
		}
	}
	p->getSprite()->draw(getOnscreenX(p, p->x, p->xOffset), getOnscreenY(p, p->y, p->yOffset));
	for (int i = p->x - WIDTH / 32 / 2 - 2; i <= p->x + WIDTH / 32 / 2 + 2; i++)
	{
		for (int j = p->y - HEIGHT / 64 - 2; j <= p->y + HEIGHT / 64 + 2; j++)
		{
			int index = world->getUpper(i, j);
			if (tileset[index] != nullptr&&index!=0) tileset[index]->draw(WIDTH / 2 - 16 - p->xOffset + 32 * (i - p->x), HEIGHT / 2 - 16 - p->yOffset + 32 * (j - p->y));
		}
	}
}

void offsetShift(Player *p)
{
	while (p->xOffset <= -32)
	{
		p->xOffset += 32;
		p->x--;
	}
	while (p->yOffset <= -32)
	{
		p->yOffset += 32;
		p->y--;
	}
	while (p->xOffset >= 32)
	{
		p->xOffset -= 32;
		p->x++;
	}
	while (p->yOffset >= 32)
	{
		p->yOffset -= 32;
		p->y++;
	}
}

void attemptMove(Player *p, int dx, int dy, int speed)
{
	for (int i = 0; i < speed; i++)
	{
		p->xOffset += dx;
		p->yOffset += dy;
		offsetShift(p);
		if (worlds[p->worldID]->collides(p->x, p->y, p->xOffset, p->yOffset, p->width, p->height))
		{
			p->xOffset -= dx;
			p->yOffset -= dy;
			break;
		}
	}
	offsetShift(p);
}

bool firstWithWorldId(int worldId, int playerId)
{
	for (int i = 0; i < numPlayers; i++)
	{
		if (getPlayer(i)->worldID == worldId)
			return i == playerId;
	}
	return false;
}

void WorldState::run()
{
	startMenu();
	int dx = 0;
	int dy = 0;
	int _dir = p->dir;
	if (keys[p->keyUp]) dy--;
	if (keys[p->keyDown]) dy++;
	if (keys[p->keyLeft]) dx--;
	if (keys[p->keyRight]) dx++;
	if (p->dir == 0 && dx > 0)
	{} else if (p->dir == 1 && dy < 0)
	{} else if (p->dir == 2 && dx < 0)
	{} else if (p->dir == 3 && dy > 0)
	{} else {
		if (dx > 0)
			p->dir = 0;
		else if (dy < 0)
			p->dir = 1;
		else if (dx < 0)
			p->dir = 2;
		else if (dy > 0)
			p->dir = 3;
	}
	if (_dir != p->dir) p->wait = 0;
	if ((dx != 0 || dy != 0) && p->wait >= 3) attemptMove(p, dx, dy, 2);
	else if (dx == 0 && dy == 0)
		p->wait = 0;
	p->wait++;
	int sight = 1;
	if (keys[p->keyA]&&!a)
	{
		bool mapInteract = false;
		if (p->dir == 0)
		{
			if(worlds[p->worldID]->interact(p, safeDiv(32 * p->x + p->xOffset + p->width + sight, 32), safeDiv(32 * p->y + p->yOffset + p->height / 2, 32)))
				mapInteract = true;
		}
		if (p->dir == 1)
		{
			if (worlds[p->worldID]->interact(p, safeDiv(32 * p->x + p->xOffset + p->width/2, 32), safeDiv(32 * p->y + p->yOffset - sight, 32)))
				mapInteract = true;
		}
		if (p->dir == 2)
		{
			if (worlds[p->worldID]->interact(p, safeDiv(32 * p->x + p->xOffset - sight, 32), safeDiv(32 * p->y + p->yOffset + p->height / 2, 32)))
				mapInteract = true;
		}
		if (p->dir == 3)
		{
			if (worlds[p->worldID]->interact(p, safeDiv(32 * p->x + p->xOffset + p->width/2, 32), safeDiv(32 * p->y + p->yOffset + p->height + sight, 32)))
				mapInteract = true;
		}
		if (!mapInteract)
		{
			//try interacting with entities
			List<Entity*> entities = worlds[p->worldID]->entities;
			int x = 0;
			int y = 0;
			if (p->dir == 0)
			{
				x = 32 * p->x + p->xOffset + p->width + sight;
				y = 32 * p->y + p->yOffset + p->height / 2;
			}
			else if (p->dir == 1)
			{
				x = 32 * p->x + p->xOffset + p->width / 2;
				y = 32 * p->y + p->yOffset - sight;
			}
			else if (p->dir == 2)
			{
				x = 32 * p->x + p->xOffset - sight;
				y = 32 * p->y + p->yOffset + p->height / 2;
			}
			else if (p->dir == 3)
			{
				x = 32 * p->x + p->xOffset + p->width / 2;
				y = 32 * p->y + p->yOffset + p->height + sight;
			}
			for (int i = 0; i < entities.length(); i++)
			{
				Entity *e = entities[i];
				if (e->isAlive)
				{
					if (rectCollides(x, y, 2, 2, 32 * e->x + e->xOffset, 32 * e->y + e->yOffset, e->width, e->height))
					{
						if (e->interact(p))
						{
							break;
						}
					}
				}
			}
		}
	}
	if (firstWithWorldId(p->worldID, p->id))
	{
		List<Entity*> &entities = worlds[p->worldID]->entities;
		for (int i = 0; i < entities.length(); i++)
		{
			if (entities[i]->isAlive)
			{
				entities[i]->run();
			}
			if (!entities[i]->isAlive)
			{
				entities.removeAt(i);
				i--;
			}
		}
	}
	endMenu();
}

class TextBox : public GameState
{
	string msg;
public:
	TextBox(Player *p, string _msg);
	void run();
	void draw();
};

TextBox::TextBox(Player *p, string _msg) : GameState(p)
{
	msg = _msg;
}

void TextBox::run()
{
	startMenu();
	if (keys[p->keyA] && !a)
	{
		p->popState();
		return;
	}
	endMenu();
}

void TextBox::draw()
{
	const int MAX_LINES = 4;
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0);
	SDL_Rect rect;
	rect.x = 0;
	rect.w = WIDTH;
	rect.h = 8 + 2 * MAX_LINES*8;
	rect.y = HEIGHT - rect.h;
	SDL_RenderFillRect(renderer, &rect);
	/*for (int i = 0; i < msg.length(); i++)
	{
		chars[msg[i]]->draw(8 + 8 * i, rect.y + 8);
	}*/
	int line = 1;
	int x = 0;
	int i = 0;
	int j = 0;
	while (j < msg.length())
	{
		while (msg[j] != ' '&&j < msg.length()) j++;
		if (x + j - i > WIDTH / 8 - 2)
		{
			line++;
			x = 0;
		}
		for (; i < j; i++)
		{
			if (!(x == 0 && msg[i] == ' '))
			{
				chars[msg[i]]->draw(8 + 8 * x, rect.y + 8 * line);
				x++;
			}
		}
		j++;
	}
}

void textbox(int i, string msg)
{
	Player *p = getPlayer(i);
	if (p != nullptr)
	{
		p->pushState(new TextBox(p, msg));
	}
	else {
		debug("Trying to access non existent player " << i);
	}
}