
class Parabola {
    public:
        double focus;
        double site;

        static Point* getIntersection(Parabola* a, Parabola* b);

    private:
        Parabola* parent;
};
