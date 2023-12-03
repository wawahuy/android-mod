import { defineStore } from 'pinia';

export const useAppStore = defineStore({
    id: 'app',
    state: () => ({
      isPageLoading: false,
    }),
    actions: {
      showPageLoading() {
        this.isPageLoading = true;
      },
      hidePageLoading() {
        this.isPageLoading = false;
      },
    }
});