#include <concord/discord.h>
#include <concord/error.h>
#include <string.h>

// Yes that is hardcoded but you won't be able to do anything with it so meh
#ifndef GUILD_ID
#define GUILD_ID 1175890585618489365
#endif

static void
on_ready(struct discord *client, const struct discord_ready *event) {
    struct discord_create_guild_application_command params = {
        .name = "ping", .description = "Ping command!"};

    discord_create_guild_application_command(
        client, event->application->id, GUILD_ID, &params, NULL
    );
}

static void on_interaction(
    struct discord *client, const struct discord_interaction *event
) {
    if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND)
        return;

    if (strcmp(event->data->name, "ping") == 0) {
        struct discord_interaction_response params = {
            .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
            .data = &(struct discord_interaction_callback_data
            ) {.content = "Bloop!", .flags = DISCORD_MESSAGE_EPHEMERAL}};
        discord_create_interaction_response(
            client, event->id, event->token, &params, NULL
        );
    }
}

int main(int argc, const char **argv) {
    struct discord *client;
    CCORDcode err;

    if (argc != 2) {
        perror("Config file missing from arguments");
        return 1;
    }
    client = discord_config_init(argv[1]);
    if (!client) {
        perror("Could not initialize client (Somehow?)");
        return 1;
    }
    discord_set_on_ready(client, &on_ready);
    discord_set_on_interaction_create(client, &on_interaction);

    err = discord_run(client);
    // TODO(emneo): Do something with the error code
    return err;
}
