import { createApp } from 'vue';
import { createPinia } from 'pinia';
import App from './App.vue';
import { router } from './router';

// Vuetify
import 'vuetify/styles'
import { createVuetify } from 'vuetify';
import * as components from 'vuetify/components';
import * as directives from 'vuetify/directives';
import './styles/vuetify.css';
import './styles/base.css';
import './styles/tailwind.css';

const vuetify = createVuetify({
    components,
    directives,
    theme: {
      defaultTheme: 'dark'
    }
})

const pinia = createPinia();
const app = createApp(App);
app.use(pinia);
app.use(vuetify);
app.use(router);
app.mount('#app');
