#include "raylib.h"
#include "../core/graph.h"
#include "../core/dijkstra.h"
#include "../core/services.h"
#include "serial.h"

#include <cmath>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace mathcity;

// city coords x:[0,20] y:[0,20] -> screen
static Vector2 to_screen(double x, double y) {
    float sx = 60 + (float)(x / 20.0 * 680);
    float sy = 560 - (float)(y / 20.0 * 480); // flip y
    return {sx, sy};
}

static float pt_seg_dist(Vector2 p, Vector2 a, Vector2 b) {
    Vector2 ab{b.x-a.x, b.y-a.y};
    float len2 = ab.x*ab.x + ab.y*ab.y;
    if (len2 == 0) return std::hypot(p.x-a.x, p.y-a.y);
    float t = ((p.x-a.x)*ab.x + (p.y-a.y)*ab.y) / len2;
    t = t < 0 ? 0 : (t > 1 ? 1 : t);
    return std::hypot(p.x-(a.x+t*ab.x), p.y-(a.y+t*ab.y));
}

static Color kind_color(const std::string& k) {
    if (k=="hospital") return RED;
    if (k=="fire") return ORANGE;
    if (k=="school") return BLUE;
    if (k=="water") return SKYBLUE;
    if (k=="waste") return BROWN;
    if (k=="residential") return GREEN;
    return DARKGRAY;
}

int main() {
    InitWindow(800, 600, "MATHCITY - decoupled");
    SetTargetFPS(60);

    std::unordered_map<int,int> traffic; // edge_id -> 1..3
    std::unordered_set<int> blocked;
    std::string src = "R1", dst = "H";
    bool emergency = false;
    std::string msg = "";

    while (!WindowShouldClose()) {
        // input: click edge to block/unblock, T cycles traffic, E emergency, R reset
        if (IsMouseButtonPressed(0)) {
            Vector2 m = GetMousePosition();
            const auto& ns = nodes();
            const auto& es = edges();
            for (const auto& e : es) {
                Vector2 a = to_screen(ns[e.u].x, ns[e.u].y);
                Vector2 b = to_screen(ns[e.v].x, ns[e.v].y);
                if (pt_seg_dist(m, a, b) < 10) {
                    if (blocked.count(e.id)) blocked.erase(e.id);
                    else blocked.insert(e.id);
                    break;
                }
            }
        }
        if (IsKeyPressed(KEY_T)) {
            Vector2 m = GetMousePosition();
            const auto& ns = nodes();
            const auto& es = edges();
            for (const auto& e : es) {
                Vector2 a = to_screen(ns[e.u].x, ns[e.u].y);
                Vector2 b = to_screen(ns[e.v].x, ns[e.v].y);
                if (pt_seg_dist(m, a, b) < 10) {
                    int t = traffic.count(e.id) ? traffic[e.id] : 1;
                    t = t % 3 + 1;
                    traffic[e.id] = t;
                    break;
                }
            }
        }
        if (IsKeyPressed(KEY_E)) { src = "F"; dst = "H"; emergency = true; }
        if (IsKeyPressed(KEY_N)) { src = "R1"; dst = "H"; emergency = false; }
        if (IsKeyPressed(KEY_R)) { blocked.clear(); traffic.clear(); emergency = false; }
        if (IsKeyPressed(KEY_W)) {
            auto top = waste_best();
            msg = "Waste best: ";
            for (int id : top[0].order) msg += nodes()[id].name + " ";
            msg += "= " + std::to_string((int)top[0].total);
        }

        RouteResult r = dijkstra(src, dst, traffic, blocked);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        const auto& ns = nodes();
        const auto& es = edges();

        // edges
        for (const auto& e : es) {
            Vector2 a = to_screen(ns[e.u].x, ns[e.u].y);
            Vector2 b = to_screen(ns[e.v].x, ns[e.v].y);
            Color c = LIGHTGRAY;
            float w = 2;
            if (blocked.count(e.id)) { c = RED; w = 4; }
            else if (traffic.count(e.id) && traffic.at(e.id) > 1) { c = YELLOW; w = 4; }
            DrawLineEx(a, b, w, c);
            // one-way arrow
            if (e.one_way) {
                Vector2 mid{(a.x+b.x)/2, (a.y+b.y)/2};
                DrawCircleV(mid, 4, DARKGRAY);
            }
        }
        // route highlight
        if (r.found) {
            for (int eid : r.edge_ids) {
                const Edge* e = nullptr;
                for (const auto& ee : es) if (ee.id == eid) { e = &ee; break; }
                if (!e) continue;
                DrawLineEx(to_screen(ns[e->u].x, ns[e->u].y),
                           to_screen(ns[e->v].x, ns[e->v].y), 5, emergency ? RED : GREEN);
            }
        }
        // nodes
        for (const auto& n : ns) {
            Vector2 p = to_screen(n.x, n.y);
            DrawCircleV(p, 10, kind_color(n.kind));
            DrawText(n.name.c_str(), (int)p.x - 8, (int)p.y - 24, 12, BLACK);
        }

        // HUD - large, two lines with background
        DrawRectangle(0, 0, 800, 62, Fade(WHITE, 0.92));
        std::string line1 = src + "->" + dst + (r.found ? " cost=" + std::to_string((int)r.total_cost) : " NO ROUTE");
        DrawText(line1.c_str(), 10, 6, 22, BLACK);
        DrawText("click=edge block | T+hover=traffic | E=emerg F->H | N=normal | R=reset | W=waste", 10, 34, 16, DARKGRAY);
        if (!msg.empty()) DrawText(msg.c_str(), 10, 572, 18, BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
