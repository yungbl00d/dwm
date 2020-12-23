/** Function to shift the current view to the left/right
 *
 * @param: "arg->i" stores the number of tags to shift right (positive value)
 *          or left (negative value)
 */
void
shiftview(const Arg *arg)
{
	Arg shifted;
	Client *c;
	unsigned int tagmask = 0;

	for (c = selmon->clients; c; c = c->next)
		if (!(c->tags & TAGMASK))
			tagmask = tagmask | c->tags;

	shifted.ui = selmon->tagset[selmon->seltags] & ~TAGMASK;
	if (arg->i > 0) /* left circular shift */
		do {
			shifted.ui = (shifted.ui << arg->i)
			   | (shifted.ui >> (LENGTH(tags) - arg->i));
			shifted.ui &= ~TAGMASK;
		} while (tagmask && !(shifted.ui & tagmask));
	else /* right circular shift */
		do {
			shifted.ui = (shifted.ui >> (- arg->i)
			   | shifted.ui << (LENGTH(tags) + arg->i));
			shifted.ui &= ~TAGMASK;
		} while (tagmask && !(shifted.ui & tagmask));

	view(&shifted);
}

void
shifttag(const Arg *arg)
{
	Arg a;
	Client *c;
	unsigned visible = 0;
	int i = arg->i;
	int count = 0;
	int nextseltags, curseltags = selmon->tagset[selmon->seltags];

	do {
		if(i > 0) // left circular shift
			nextseltags = (curseltags << i) | (curseltags >> (LENGTH(tags) - i));

		else // right circular shift
			nextseltags = curseltags >> (- i) | (curseltags << (LENGTH(tags) + i));

                // Check if tag is visible
		for (c = selmon->clients; c && !visible; c = c->next)
			if (nextseltags & c->tags) {
				visible = 1;
				break;
			}
		i += arg->i;
	} while (!visible && ++count < 10);

	if (count < 10) {
		a.i = nextseltags;
		tag(&a);
	}
}

void
tagtoleft(const Arg *arg) {
	if(selmon->sel != NULL
		&& __builtin_popcount(selmon->tagset[selmon->seltags] & TAGMASK) == 1
		&& selmon->tagset[selmon->seltags] > 1) {
			selmon->sel->tags >>= 1;
				focus(NULL);
				arrange(selmon);
			}
	}
	
void
tagtoright(const Arg *arg) {
	if(selmon->sel != NULL
		&& __builtin_popcount(selmon->tagset[selmon->seltags] & TAGMASK) == 1
				&& selmon->tagset[selmon->seltags] & (TAGMASK >> 1)) {
				selmon->sel->tags <<= 1;
				focus(NULL);
				arrange(selmon);
			}
	}
		
				 
void
viewtoleft(const Arg *arg) {
	if(__builtin_popcount(selmon->tagset[selmon->seltags] & TAGMASK) == 1
		&& selmon->tagset[selmon->seltags] > 1) {
		selmon->seltags ^= 1; /* toggle sel tagset */
		selmon->tagset[selmon->seltags] = selmon->tagset[selmon->seltags ^ 1] >> 1;
		focus(NULL);
		arrange(selmon);
	}
}
				
void
viewtoright(const Arg *arg) {
	if(__builtin_popcount(selmon->tagset[selmon->seltags] & TAGMASK) == 1
		&& selmon->tagset[selmon->seltags] & (TAGMASK >> 1)) {
		selmon->seltags ^= 1; /* toggle sel tagset */
		selmon->tagset[selmon->seltags] = selmon->tagset[selmon->seltags ^ 1] << 1;
		focus(NULL);
		arrange(selmon);
	}
}
				
