class GoldRushAction{
    public:
        void setNext(GoldRushAction action);
        void setLast(GoldRushAction action);
        GoldRushAction run();
    private:
        GoldRushAction nextAction;
}