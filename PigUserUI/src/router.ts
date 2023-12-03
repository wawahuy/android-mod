import { createRouter, createWebHistory, RouteRecordRaw } from 'vue-router';
import { useAppStore } from './stores/app.store';

const routes: Readonly<RouteRecordRaw[]> = [
  {
    path: '',
    component: () => import('./pages/Home.vue'),
    meta: { noAuth: true },
  },
  {
    path: '/:pathMatch(.*)*',
    redirect: '/',
    meta: { noAuth: true },
  },
]

export const router = createRouter({
  history: createWebHistory(),
  routes,
});

router.beforeEach(async (to, prev, next) => {
  const pageStore = useAppStore();
  let showPageLoading = true;
  if (prev && prev.path == to.path) {
    showPageLoading = false;
  }
  if (showPageLoading) {
    pageStore.showPageLoading();
  }
  next();
});

router.afterEach(async (_to, _, _fail) => {
  const pageStore = useAppStore();
  pageStore.hidePageLoading();
})