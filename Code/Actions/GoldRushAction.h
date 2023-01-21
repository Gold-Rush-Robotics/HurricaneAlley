
class GoldRushAction{
    public:
        void setNext(GoldRushAction action);
        void setLast(GoldRushAction action);
        virtual GoldRushAction run();
    private:
        GoldRushAction nextAction;
}