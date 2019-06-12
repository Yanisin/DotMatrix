#include <hw/disp.h>
#include <util/icons.h>

static bool get_bool(int x, int y)
{
	return disp_get(x, y) >= 16;
}

static void set_bool(int x, int y, bool val)
{
	disp_set_noupdate(x, y, val ? 31 : 0);
}

void draw_icon(icon_data icon, enum blit_ops ops)
{
	for(int i = 0; i < 8*8; i++) {
		int x = i % 8;
		int y = i / 8;
		bool val = ((icon[y] >> x) & 1) != 0;

		switch (ops) {
		case BLIT_SET:
			set_bool(x, y, val);
			break;
		case BLIT_NOT:
			set_bool(x, y, !val);
			break;
		case BLIT_OR:
			set_bool(x, y, val || get_bool(x, y));
			break;
		case BLIT_AND:
			set_bool(x, y, val || get_bool(x, y));
			break;
		case BLIT_XOR:
			set_bool(x, y, val ^ get_bool(x, y));
			break;
		}

	}
	disp_update_rows();
}

#define _ 0
#define X 1

#if 0
icon_data template = {
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
};

icon_data template_nomacro = {
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
};
#endif

icon_data icon_arrow = {
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, X, X, _, _, _),
	ICON_ROW(_, _, _, X, X, _, _, _),
	ICON_ROW(_, _, X, _, _, X, _, _),
	ICON_ROW(_, _, X, _, _, X, _, _),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
};

icon_data icon_edge_line = {
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
};

icon_data icon_top_edges = {
	ICON_ROW(X, _, _, _, _, _, _, X),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
};

icon_data icon_square = {
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
};

icon_data icon_master = {
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, X, _, X, X, _, X, _),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(_, X, _, X, X, _, X, _),
	ICON_ROW(_, X, _, X, X, _, X, _),
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
};

icon_data  smiley = {
	ICON_ROW(_, _, X, X, X, X, _, _),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(X, _, X, _, _, X, _, X),
	ICON_ROW(X, _, X, _, _, X, _, X),
	ICON_ROW(X, X, _, _, _, _, X, X),
	ICON_ROW(X, _, X, X, X, X, _, X),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(_, _, X, X, X, X, _, _),
};

icon_data  smiley_sad = {
	ICON_ROW(_, _, X, X, X, X, _, _),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(X, _, X, _, _, X, _, X),
	ICON_ROW(X, _, _, _, _, _, _, X),
	ICON_ROW(X, _, X, X, X, X, _, X),
	ICON_ROW(X, X, _, _, _, _, X, X),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(_, _, X, X, X, X, _, _),
};
