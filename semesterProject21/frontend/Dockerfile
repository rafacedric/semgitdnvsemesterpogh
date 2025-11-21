FROM node:22

WORKDIR /app

COPY deskApp/package*.json ./

RUN npm ci --legacy-peer-deps || npm install --legacy-peer-deps

COPY deskApp/ ./

CMD ["npm", "run", "dev"]