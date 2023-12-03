import { createApp } from 'vue';
import { createPinia } from 'pinia';
import App from './App.vue';
import { router } from './router';

// Vuetify
import 'vuetify/styles'
import { createVuetify } from 'vuetify';
import * as components from 'vuetify/components';
import * as directives from 'vuetify/directives';
import './styles/base.scss';
import './styles/tailwind.scss';

const vuetify = createVuetify({
    components,
    directives,
})

const pinia = createPinia();
const app = createApp(App);
app.use(pinia);
app.use(vuetify);
app.use(router);
app.mount('#app');
